#include "VCycle.h"
#include "LinearSolver.h"

std::vector<double> VCycle::solve(std::vector<double> u,
                                    const std::vector<double>& f,
                                    std::shared_ptr<Grid1D> grid,
                                    int nu1, int nu2) const {
    
    int n = grid->getN();
    int N_total = grid->getNumTotal();
    
    // 预平滑
    u = smoother(u, f, grid, nu1);
    
    // 最粗网格：直接求解
    if (grid->isCoarsest()) {
        double h = grid->getH();
        double inv_h2 = 1.0 / (h * h);
        
        std::vector<std::vector<double>> A(N_total, std::vector<double>(N_total, 0.0));
        std::vector<double> rhs = f;
        
        // 内部点
        for (int i = 1; i < n; i++) {
            A[i][i-1] = -inv_h2;
            A[i][i]   =  2.0 * inv_h2;
            A[i][i+1] = -inv_h2;
        }
        
        // 左边界
        if (left_type == "dirichlet") {
            A[0][0] = 1.0;
        } else if (left_type == "neumann") {
            A[0][0] =  2.0 * inv_h2;
            A[0][1] = -2.0 * inv_h2;
        }
        
        // 右边界
        if (right_type == "dirichlet") {
            A[n][n] = 1.0;
        } else if (right_type == "neumann") {
            A[n][n-1] = -2.0 * inv_h2;
            A[n][n]   =  2.0 * inv_h2;
        }
        
        return LinearSolver::gaussianElimination(A, rhs);
    }
    
    // 计算残差
    std::vector<double> Au = smoother.matVec(u, grid);
    std::vector<double> r(N_total, 0.0);
    for (int i = 0; i < N_total; i++) {
        r[i] = f[i] - Au[i];
    }
    
    // 限制残差到粗网格
    auto coarse = grid->getCoarseGrid();
    std::vector<double> r_coarse = restrictor(r, grid, coarse);
    
    // 在粗网格上递归求解残差方程
    std::vector<double> e_coarse(coarse->getNumTotal(), 0.0);
    e_coarse = solve(e_coarse, r_coarse, coarse, nu1, nu2);
    
    // 延拓误差并校正
    std::vector<double> e = prolongator(e_coarse, coarse, grid);
    for (int i = 0; i < N_total; i++) {
        u[i] += e[i];
    }
    
    // 后平滑
    u = smoother(u, f, grid, nu2);
    
    return u;
}