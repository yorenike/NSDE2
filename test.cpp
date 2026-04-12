#include <iostream>
#include <vector>
#include "LinearSolver.h"

int main() {
    double h = 0.125;
    double inv_h2 = 1.0 / (h * h);
    int n = 8;
    int N = n + 1;
    
    // 对于 exp_y_sin_x
    double f0 = -2.0;      // f(0)
    double sigma = -2.0;   // 外法向导数 = -u'(0), u'(0)=2
    
    std::vector<std::vector<double>> A(N, std::vector<double>(N, 0.0));
    std::vector<double> b(N, 0.0);
    
    // 内部点
    for (int i = 1; i < n; i++) {
        A[i][i-1] = -inv_h2;
        A[i][i] = 2.0 * inv_h2;
        A[i][i+1] = -inv_h2;
        b[i] = 2.0;  // 对于 quadratic 是 2，对于 exp_y_sin_x 不是常数
    }
    
    // 左边界 Neumann
    A[0][0] = inv_h2;
    A[0][1] = -inv_h2;
    b[0] = f0 / 2.0 - sigma / h;
    
    // 右边界 Dirichlet
    A[n][n] = 1.0;
    b[n] = 0.0;
    
    std::vector<double> u = LinearSolver::gaussianElimination(A, b);
    
    std::cout << "Solution at x=0: " << u[0] << std::endl;
    std::cout << "Expected: 1" << std::endl;
    
    return 0;
}