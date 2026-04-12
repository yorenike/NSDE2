#include "Smoother.h"
#include <cmath>

std::vector<double> Smoother::matVec(const std::vector<double>& u,
                                       std::shared_ptr<Grid1D> grid) const {
    int n = grid->getN();
    double h = grid->getH();
    double inv_h2 = 1.0 / (h * h);
    std::vector<double> Au(grid->getNumTotal(), 0.0);
    
    // 内部点
    for (int i = 1; i < n; i++) {
        Au[i] = (-u[i-1] + 2.0*u[i] - u[i+1]) * inv_h2;
    }
    
    // 左边界
    if (left_type == "dirichlet") {
        Au[0] = u[0];
    } else if (left_type == "neumann") {
        Au[0] = 2.0 * (u[0] - u[1]) * inv_h2;
    }
    
    // 右边界
    if (right_type == "dirichlet") {
        Au[n] = u[n];
    } else if (right_type == "neumann") {
        Au[n] = 2.0 * (u[n] - u[n-1]) * inv_h2;
    }
    
    return Au;
}

std::vector<double> Smoother::operator()(const std::vector<double>& u0,
                                           const std::vector<double>& f,
                                           std::shared_ptr<Grid1D> grid,
                                           int nu) const {
    std::vector<double> u = u0;
    int n = grid->getN();
    double h2 = grid->getH() * grid->getH();
    double inv_h2 = 1.0 / h2;
    
    for (int iter = 0; iter < nu; iter++) {
        std::vector<double> u_new = u;
        
        // 左边界
        if (left_type == "dirichlet") {
            double r0 = f[0] - u[0];
            u_new[0] = u[0] + omega * r0;
        } else if (left_type == "neumann") {
            double Au0 = 2.0 * (u[0] - u[1]) * inv_h2;
            double r0 = f[0] - Au0;
            u_new[0] = u[0] + omega * (h2 / 2.0) * r0;
        }
        
        // 内部点
        for (int i = 1; i < n; i++) {
            double Au_i = (-u[i-1] + 2.0*u[i] - u[i+1]) * inv_h2;
            double r = f[i] - Au_i;
            u_new[i] = u[i] + omega * (h2 / 2.0) * r;
        }
        
        // 右边界
        if (right_type == "dirichlet") {
            double rn = f[n] - u[n];
            u_new[n] = u[n] + omega * rn;
        } else if (right_type == "neumann") {
            double Au_n = 2.0 * (u[n] - u[n-1]) * inv_h2;
            double rn = f[n] - Au_n;
            u_new[n] = u[n] + omega * (h2 / 2.0) * rn;
        }
        
        u = u_new;
    }
    
    return u;
}