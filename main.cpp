#include <iostream>
#include <iomanip>
#include <cmath>
#include "Grid1D.h"
#include "Restrictor.h"
#include "Prolongator.h"
#include "Smoother.h"
#include "VCycle.h"
#include "FMG.h"
#include "TestFunction.h"
#include "JSONParser.h"

double computeError(const std::vector<double>& u, 
                    std::shared_ptr<Grid1D> grid,
                    const TestFunction& tf) {
    double error = 0.0;
    for (int i = 0; i <= grid->getN(); i++) {
        double x = grid->getX(i);
        double u_exact = tf.exact1D(x);
        error = std::max(error, std::abs(u[i] - u_exact));
    }
    return error;
}

double computeConvergenceRate(double error1, double error2, double h1, double h2) {
    return std::log(error1 / error2) / std::log(h1 / h2);
}

int main(int argc, char* argv[]) {
    std::cout << "========================================" << std::endl;
    std::cout << " 1D Multigrid Solver for Poisson Equation" << std::endl;
    std::cout << "========================================" << std::endl;
    
    std::string config_file = "input.json";
    if (argc > 1) {
        config_file = argv[1];
    }
    
    JSONParser parser(config_file);
    
    int n = parser.getInt("grid.n");
    std::string test_name = parser.getString("test_function.name");
    std::string restriction_type = parser.getString("multigrid.restriction");
    std::string interpolation_type = parser.getString("multigrid.interpolation");
    std::string cycle_type = parser.getString("multigrid.cycle");
    int nu1 = parser.getInt("multigrid.nu1");
    int nu2 = parser.getInt("multigrid.nu2");
    double epsilon = parser.getDouble("multigrid.epsilon");
    int max_iter = parser.getInt("multigrid.max_iter");
    
    std::cout << "\nConfiguration:" << std::endl;
    std::cout << "  n = " << n << std::endl;
    std::cout << "  test_function = " << test_name << std::endl;
    std::cout << "  restriction = " << restriction_type << std::endl;
    std::cout << "  interpolation = " << interpolation_type << std::endl;
    std::cout << "  cycle = " << cycle_type << std::endl;
    std::cout << "  nu1 = " << nu1 << ", nu2 = " << nu2 << std::endl;
    std::cout << "  epsilon = " << epsilon << std::endl;
    
    TestFunction tf(test_name);
    auto grid = std::make_shared<Grid1D>(n, 0.0, 1.0);
    
    // 右端项 f
    std::vector<double> f(grid->getNumTotal(), 0.0);
    for (int i = 1; i < n; i++) {
        double x = grid->getX(i);
        f[i] = tf.f1D(x);
    }
    // 边界点：存储边界条件值
    f[0] = tf.exact1D(0.0);
    f[n] = tf.exact1D(1.0);
    
    // 初始猜测（设置正确的边界值）
    std::vector<double> u(grid->getNumTotal(), 0.0);
    u[0] = tf.exact1D(0.0);
    u[n] = tf.exact1D(1.0);
    
    Restrictor restrictor;
    Prolongator prolongator;
    Smoother smoother(2.0/3.0);
    VCycle vcycle(restrictor, prolongator, smoother);
    FMG fmg(restrictor, prolongator, smoother, vcycle);
    
    std::cout << "\nSolving..." << std::endl;
    std::vector<double> result = fmg.solve(f, grid, nu1, nu2);
    
    double error = computeError(result, grid, tf);
    std::cout << "\nResult:" << std::endl;
    std::cout << "  Max error = " << std::scientific << error << std::endl;
    
    std::cout << "\nSolution at selected points:" << std::endl;
    std::cout << std::fixed << std::setprecision(6);
    int step = n / 8;
    if (step < 1) step = 1;
    for (int i = 0; i <= n; i += step) {
        double x = grid->getX(i);
        double u_exact = tf.exact1D(x);
        std::cout << "  x = " << x << ": numerical = " << result[i] 
                  << ", exact = " << u_exact 
                  << ", error = " << std::abs(result[i] - u_exact) << std::endl;
    }
    
    // 收敛率测试
    std::cout << "\n========================================" << std::endl;
    std::cout << "Convergence Rate Test" << std::endl;
    std::cout << "========================================" << std::endl;
    
    std::vector<int> ns = {16, 32, 64, 128};
    std::vector<double> errors;
    
    for (int n_test : ns) {
        std::cout << "\nTesting n = " << n_test << ":" << std::endl;
        auto grid_test = std::make_shared<Grid1D>(n_test, 0.0, 1.0);
        
        std::vector<double> f_test(grid_test->getNumTotal(), 0.0);
        for (int i = 1; i < n_test; i++) {
            double x = grid_test->getX(i);
            f_test[i] = tf.f1D(x);
        }
        f_test[0] = tf.exact1D(0.0);
        f_test[n_test] = tf.exact1D(1.0);
        
        FMG fmg_test(restrictor, prolongator, smoother, vcycle);
        std::vector<double> u_test = fmg_test.solve(f_test, grid_test, nu1, nu2);
        
        double err = computeError(u_test, grid_test, tf);
        errors.push_back(err);
        
        std::cout << "  error = " << std::scientific << err << std::endl;
    }
    
    std::cout << "\nConvergence rates:" << std::endl;
    for (size_t i = 1; i < ns.size(); i++) {
        double h1 = 1.0 / ns[i-1];
        double h2 = 1.0 / ns[i];
        double rate = computeConvergenceRate(errors[i-1], errors[i], h1, h2);
        std::cout << "  " << ns[i-1] << " -> " << ns[i] << ": rate = " 
                  << std::fixed << std::setprecision(2) << rate << std::endl;
    }
    
    std::cout << "\n========================================" << std::endl;
    std::cout << "Done!" << std::endl;
    std::cout << "========================================" << std::endl;
    
    return 0;
}