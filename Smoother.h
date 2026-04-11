#ifndef SMOOTHER_H
#define SMOOTHER_H

#include <vector>
#include <memory>
#include "Grid1D.h"

class Smoother {
private:
    double omega;
    
public:
    Smoother(double omega_ = 2.0/3.0) : omega(omega_) {}
    
    // 加权 Jacobi 迭代
    std::vector<double> operator()(const std::vector<double>& u0,
                                    const std::vector<double>& f,
                                    std::shared_ptr<Grid1D> grid,
                                    int nu) const;
    
    // 计算 A*u（离散 Laplacian）
    std::vector<double> matVec(const std::vector<double>& u,
                                std::shared_ptr<Grid1D> grid) const;
};

#endif