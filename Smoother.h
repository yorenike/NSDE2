#ifndef SMOOTHER_H
#define SMOOTHER_H

#include <vector>
#include <memory>
#include <string>
#include "Grid1D.h"

class Smoother {
private:
    double omega;
    std::string left_type;   // "dirichlet" 或 "neumann"
    std::string right_type;  // "dirichlet" 或 "neumann"
    
public:
    Smoother(double omega_ = 2.0/3.0,
             const std::string& lt = "dirichlet",
             const std::string& rt = "dirichlet")
        : omega(omega_), left_type(lt), right_type(rt) {}
    
    // 计算 A*u（离散 Laplacian，考虑边界条件）
    std::vector<double> matVec(const std::vector<double>& u,
                                std::shared_ptr<Grid1D> grid) const;
    
    // 加权 Jacobi 平滑迭代
    std::vector<double> operator()(const std::vector<double>& u0,
                                    const std::vector<double>& f,
                                    std::shared_ptr<Grid1D> grid,
                                    int nu) const;
};

#endif