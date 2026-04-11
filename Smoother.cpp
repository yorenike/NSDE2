#include "Smoother.h"

std::vector<double> Smoother::matVec(const std::vector<double>& u,
                                       std::shared_ptr<Grid1D> grid) const {
    int n = grid->getN();
    double inv_h2 = 1.0 / (grid->getH() * grid->getH());
    std::vector<double> Au(grid->getNumTotal(), 0.0);
    
    for (int i = 1; i < n; i++) {
        Au[i] = (-u[i-1] + 2.0*u[i] - u[i+1]) * inv_h2;
    }
    Au[0] = u[0];
    Au[n] = u[n];
    
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
        
        for (int i = 1; i < n; i++) {
            double r = f[i] - (-u[i-1] + 2.0*u[i] - u[i+1]) * inv_h2;
            u_new[i] = u[i] + omega * (h2 / 2.0) * r;
        }
        
        u = u_new;
    }
    
    return u;
}