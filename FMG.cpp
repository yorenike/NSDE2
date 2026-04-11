#include "FMG.h"
#include "LinearSolver.h"
#include <iostream>

std::vector<double> FMG::solve(const std::vector<double>& f,
                                 std::shared_ptr<Grid1D> grid,
                                 int nu1, int nu2) const {
    
    int n = grid->getN();
    std::cout << "FMG::solve: n = " << n << std::endl;
    
    if (grid->isCoarsest()) {
        std::cout << "  Solving on coarsest grid (n = " << n << ")" << std::endl;
        
        int N_total = grid->getNumTotal();
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
    
    auto coarse = grid->getCoarseGrid();
    if (!coarse) {
        std::cerr << "Error: Failed to create coarse grid!" << std::endl;
        exit(1);
    }
    std::cout << "  Coarse grid n = " << coarse->getN() << std::endl;
    
    std::vector<double> f_coarse = restrictor(f, grid, coarse);
    std::vector<double> u_coarse = solve(f_coarse, coarse, nu1, nu2);
    std::vector<double> u = prolongator(u_coarse, coarse, grid);
    u = vcycle.solve(u, f, grid, nu1, nu2);
    
    return u;
}