#include "VCycle.h"
#include "LinearSolver.h"
#include <iostream>

std::vector<double> VCycle::solve(std::vector<double> u,
                                    const std::vector<double>& f,
                                    std::shared_ptr<Grid1D> grid,
                                    int nu1, int nu2) const {
    
    int n = grid->getN();
    int N_total = grid->getNumTotal();
    
    u = smoother(u, f, grid, nu1);
    
    if (grid->isCoarsest()) {
        std::cout << "  VCycle: Solving on coarsest grid (n = " << n << ")" << std::endl;
        
        std::vector<std::vector<double>> A(N_total, std::vector<double>(N_total, 0.0));
        std::vector<double> rhs = f;
        double h = grid->getH();
        double inv_h2 = 1.0 / (h * h);
        
        for (int i = 1; i < n; i++) {
            A[i][i-1] = -inv_h2;
            A[i][i]   =  2.0 * inv_h2;
            A[i][i+1] = -inv_h2;
        }
        
        A[0][0] = 1.0;
        A[n][n] = 1.0;
        
        return LinearSolver::gaussianElimination(A, rhs);
    }
    
    std::vector<double> Au = smoother.matVec(u, grid);
    std::vector<double> r(N_total, 0.0);
    for (int i = 0; i < N_total; i++) {
        r[i] = f[i] - Au[i];
    }
    
    auto coarse = grid->getCoarseGrid();
    std::vector<double> r_coarse = restrictor(r, grid, coarse);
    std::vector<double> e_coarse(coarse->getNumTotal(), 0.0);
    e_coarse = solve(e_coarse, r_coarse, coarse, nu1, nu2);
    std::vector<double> e = prolongator(e_coarse, coarse, grid);
    
    for (int i = 0; i < N_total; i++) {
        u[i] += e[i];
    }
    
    u = smoother(u, f, grid, nu2);
    
    return u;
}