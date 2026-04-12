#include "FMG.h"
#include "LinearSolver.h"
#include <iostream>

std::vector<double> FMG::solve(const std::vector<double>& f,
                                 std::shared_ptr<Grid1D> grid,
                                 int nu1, int nu2) const {
    
    int n = grid->getN();
    int N_total = grid->getNumTotal();
    
    // 最粗网格：直接求解
    if (grid->isCoarsest()) {
        double h = grid->getH();
        double inv_h2 = 1.0 / (h * h);
        
        std::vector<std::vector<double>> A(N_total, std::vector<double>(N_total, 0.0));
        std::vector<double> rhs = f;
        
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
    
    // 1. 限制右端项到粗网格
    auto coarse = grid->getCoarseGrid();
    std::vector<double> f_coarse = restrictor(f, grid, coarse);
    
    // 2. 递归调用 FMG 求解粗网格
    std::vector<double> u_coarse = solve(f_coarse, coarse, nu1, nu2);
    
    // 3. 延拓作为细网格的初始猜测
    std::vector<double> u = prolongator(u_coarse, coarse, grid);
    
    // 4. 在细网格上做一次 V-cycle
    u = vcycle.solve(u, f, grid, nu1, nu2);
    
    return u;
}