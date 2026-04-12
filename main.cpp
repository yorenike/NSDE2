#include <iostream>
#include <iomanip>
#include <cmath>
#include <memory>
#include <vector>
#include "Grid1D.h"
#include "Restrictor.h"
#include "Prolongator.h"
#include "Smoother.h"
#include "VCycle.h"
#include "FMG.h"
#include "TestFunction.h"
#include "JSONParser.h"
#include <chrono>

// 计算误差（最大范数）
double computeError(const std::vector<double>& u, 
                    std::shared_ptr<Grid1D> grid,
                    const TestFunction1D& tf) {
    double error = 0.0;
    for (int i = 0; i <= grid->getN(); i++) {
        double x = grid->getX(i);
        double u_exact = tf.exact(x);
        error = std::max(error, std::abs(u[i] - u_exact));
    }
    return error;
}

// 计算收敛率
double computeConvergenceRate(double error1, double error2, double h1, double h2) {
    return std::log(error1 / error2) / std::log(h1 / h2);
}

// 计算残差（L2 范数，与教材一致）
double computeResidual(const std::vector<double>& u,
                       const std::vector<double>& f,
                       std::shared_ptr<Grid1D> grid,
                       const Smoother& smoother) {
    std::vector<double> Au = smoother.matVec(u, grid);
    double sum = 0.0;
    double h = grid->getH();
    for (size_t i = 0; i < Au.size(); i++) {
        double diff = f[i] - Au[i];
        sum += diff * diff;
    }
    return std::sqrt(h * sum);
}

// 运行 V-cycle 循环并输出详细信息
void runVCycle(std::shared_ptr<Grid1D> grid,
               const std::vector<double>& b,
               const TestFunction1D& tf,
               const Restrictor& restrictor,
               const Prolongator& prolongator,
               const Smoother& smoother,
               int nu1, int nu2,
               double epsilon, int max_iter,
               const std::string& left_type,
               const std::string& right_type) {
    
    VCycle vcycle(restrictor, prolongator, smoother, left_type, right_type);
    std::vector<double> u(grid->getNumTotal(), 0.0);
    
    std::cout << "\n  V-cycle convergence history:" << std::endl;
    std::cout << "  " << std::setw(4) << "Iter" 
              << std::setw(15) << "Residual" 
              << std::setw(15) << "Reduction" << std::endl;
    std::cout << "  " << std::string(34, '-') << std::endl;
    
    double prev_residual = computeResidual(u, b, grid, smoother);
    
    for (int iter = 0; iter < max_iter; iter++) {
        u = vcycle.solve(u, b, grid, nu1, nu2);
        
        double residual = computeResidual(u, b, grid, smoother);
        double reduction = (iter == 0) ? 1.0 : residual / prev_residual;
        
        std::cout << "  " << std::setw(4) << iter + 1
                  << std::setw(15) << std::scientific << residual
                  << std::setw(15) << std::fixed << std::setprecision(4) << reduction
                  << std::endl;
        
        if (residual < epsilon) {
            std::cout << "\n  Converged after " << iter + 1 << " V-cycles" << std::endl;
            break;
        }
        prev_residual = residual;
    }
    
    double error = computeError(u, grid, tf);
    std::cout << "\n  Max error = " << std::scientific << error << std::endl;
}

// 运行 FMG 并输出详细信息
void runFMG(std::shared_ptr<Grid1D> grid,
            const std::vector<double>& b,
            const TestFunction1D& tf,
            const Restrictor& restrictor,
            const Prolongator& prolongator,
            const Smoother& smoother,
            int nu1, int nu2,
            const std::string& left_type,
            const std::string& right_type) {
    
    VCycle vcycle(restrictor, prolongator, smoother, left_type, right_type);
    FMG fmg(restrictor, prolongator, smoother, vcycle, left_type, right_type);
    
    std::cout << "\n  FMG solving (single call):" << std::endl;
    
    // 记录开始时间
    auto start = std::chrono::high_resolution_clock::now();
    
    std::vector<double> u = fmg.solve(b, grid, nu1, nu2);
    
    // 记录结束时间
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    double residual = computeResidual(u, b, grid, smoother);
    double error = computeError(u, grid, tf);
    
    std::cout << "  Final residual = " << std::scientific << residual << std::endl;
    std::cout << "  Max error = " << std::scientific << error << std::endl;
    std::cout << "  Time = " << duration.count() << " microseconds" << std::endl;
}

// 收敛率测试（支持 V-cycle 和 FMG）
void convergenceRateTest(const TestFunction1D& tf,
                         const Restrictor& restrictor,
                         const Prolongator& prolongator,
                         const Smoother& smoother,
                         int nu1, int nu2,
                         double epsilon, int max_iter,
                         const std::string& left_type,
                         const std::string& right_type,
                         const std::string& cycle_type) {
    
    std::cout << "\n========================================" << std::endl;
    std::cout << "Convergence Rate Test (" << cycle_type << ")" << std::endl;
    std::cout << "========================================" << std::endl;
    
    std::vector<int> ns = {16, 32, 64, 128};
    std::vector<double> errors;
    std::vector<double> hs;
    
    for (int n_test : ns) {
        std::cout << "\nTesting n = " << n_test << ":" << std::endl;
        
        auto grid = std::make_shared<Grid1D>(n_test, 0.0, 1.0);
        double h = grid->getH();
        hs.push_back(h);
        
        // 边界值
        double left_dirichlet = tf.exact(0.0);
        double right_dirichlet = tf.exact(1.0);
        double left_neumann = -tf.du(0.0);
        double right_neumann = tf.du(1.0);
        
        // 构建右端项
        std::vector<double> b(grid->getNumTotal(), 0.0);
        for (int i = 1; i < n_test; i++) {
            b[i] = tf.f(grid->getX(i));
        }
        
        double x0 = grid->getX(0);
        if (left_type == "dirichlet") {
            b[0] = left_dirichlet;
        } else if (left_type == "neumann") {
            b[0] = tf.f(x0) + 2.0 * left_neumann / h;
        }
        
        double xn = grid->getX(n_test);
        if (right_type == "dirichlet") {
            b[n_test] = right_dirichlet;
        } else if (right_type == "neumann") {
            b[n_test] = tf.f(xn) + 2.0 * right_neumann / h;
        }
        
        double error;
        
        if (cycle_type == "V" || cycle_type == "VCycle") {
            // V-cycle: 多次迭代直到收敛
            VCycle vcycle(restrictor, prolongator, smoother, left_type, right_type);
            std::vector<double> u(grid->getNumTotal(), 0.0);
            
            for (int iter = 0; iter < max_iter; iter++) {
                u = vcycle.solve(u, b, grid, nu1, nu2);
                double residual = computeResidual(u, b, grid, smoother);
                if (residual < epsilon) break;
            }
            error = computeError(u, grid, tf);
            
        } else {
            // FMG: 一次调用
            VCycle vcycle(restrictor, prolongator, smoother, left_type, right_type);
            FMG fmg(restrictor, prolongator, smoother, vcycle, left_type, right_type);
            std::vector<double> u = fmg.solve(b, grid, nu1, nu2);
            error = computeError(u, grid, tf);
        }
        
        errors.push_back(error);
        std::cout << "  Final error = " << std::scientific << error << std::endl;
    }
    
    // 计算收敛率
    std::cout << "\nConvergence rates (expecting ~2.0 for 2nd order):" << std::endl;
    for (size_t i = 1; i < ns.size(); i++) {
        double rate = computeConvergenceRate(errors[i-1], errors[i], hs[i-1], hs[i]);
        std::cout << "  " << ns[i-1] << " -> " << ns[i] << ": rate = " 
                  << std::fixed << std::setprecision(2) << rate << std::endl;
    }
}

int main(int argc, char* argv[]) {
    std::cout << "========================================" << std::endl;
    std::cout << " 1D Multigrid Solver for Poisson Equation" << std::endl;
    std::cout << "========================================" << std::endl;
    
    // 读取配置文件
    std::string config_file = "input.json";
    if (argc > 1) {
        config_file = argv[1];
    }
    
    JSONParser parser(config_file);
    
    // 获取参数
    int n = parser.getInt("grid.n");
    std::string test_name = parser.getString("test_function.name");
    std::string left_type = parser.getString("boundary.left.type");
    std::string right_type = parser.getString("boundary.right.type");
    std::string restriction_type = parser.getStringOrDefault("multigrid.restriction", "full_weighting");
    std::string interpolation_type = parser.getStringOrDefault("multigrid.interpolation", "linear");
    std::string cycle_type = parser.getStringOrDefault("multigrid.cycle", "FMG");
    int nu1 = parser.getIntOrDefault("multigrid.nu1", 2);
    int nu2 = parser.getIntOrDefault("multigrid.nu2", 2);
    double epsilon = parser.getDoubleOrDefault("multigrid.epsilon", 1e-8);
    int max_iter = parser.getIntOrDefault("multigrid.max_iter", 100);
    
    std::cout << "\nConfiguration:" << std::endl;
    std::cout << "  n = " << n << std::endl;
    std::cout << "  test_function = " << test_name << std::endl;
    std::cout << "  left boundary: " << left_type << std::endl;
    std::cout << "  right boundary: " << right_type << std::endl;
    std::cout << "  restriction = " << restriction_type << std::endl;
    std::cout << "  interpolation = " << interpolation_type << std::endl;
    std::cout << "  cycle = " << cycle_type << std::endl;
    std::cout << "  nu1 = " << nu1 << ", nu2 = " << nu2 << std::endl;
    std::cout << "  epsilon = " << epsilon << std::endl;
    
    // 创建测试函数
    TestFunction1D tf(test_name);
    
    // 获取边界值
    double left_dirichlet = tf.exact(0.0);
    double right_dirichlet = tf.exact(1.0);
    double left_neumann = -tf.du(0.0);
    double right_neumann = tf.du(1.0);
    
    std::cout << "\nBoundary values (from test function):" << std::endl;
    std::cout << "  u(0) = " << left_dirichlet << ", u'(0) = " << left_neumann << std::endl;
    std::cout << "  u(1) = " << right_dirichlet << ", u'(1) = " << right_neumann << std::endl;
    
    // 创建求解器组件
    Restrictor restrictor(left_type, right_type);
    Prolongator prolongator;
    Smoother smoother(2.0/3.0, left_type, right_type);
    
    // 运行主测试
    auto grid = std::make_shared<Grid1D>(n, 0.0, 1.0);
    double h = grid->getH();
    
    // 构建右端项
    std::vector<double> b(grid->getNumTotal(), 0.0);
    for (int i = 1; i < n; i++) {
        b[i] = tf.f(grid->getX(i));
    }
    
    double x0 = grid->getX(0);
    if (left_type == "dirichlet") {
        b[0] = left_dirichlet;
    } else if (left_type == "neumann") {
        b[0] = tf.f(x0) + 2.0 * left_neumann / h;
    }
    
    double xn = grid->getX(n);
    if (right_type == "dirichlet") {
        b[n] = right_dirichlet;
    } else if (right_type == "neumann") {
        b[n] = tf.f(xn) + 2.0 * right_neumann / h;
    }
    
    if (cycle_type == "V" || cycle_type == "VCycle") {
        runVCycle(grid, b, tf, restrictor, prolongator, smoother, 
                  nu1, nu2, epsilon, max_iter, left_type, right_type);
    } else {
        runFMG(grid, b, tf, restrictor, prolongator, smoother, 
               nu1, nu2, left_type, right_type);
    }
    
    // 收敛率测试
    convergenceRateTest(tf, restrictor, prolongator, smoother,
                       nu1, nu2, epsilon, max_iter, left_type, right_type, cycle_type);
    
    return 0;
}