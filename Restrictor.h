#ifndef RESTRICTOR_H
#define RESTRICTOR_H

#include <vector>
#include <memory>
#include <string>
#include "Grid1D.h"

class Restrictor {
public:
    // 默认构造函数
    Restrictor() = default;
    
    // 带边界类型的构造函数
    Restrictor(const std::string& lt, const std::string& rt)
        : left_type(lt), right_type(rt) {}
    
    // 全加权限制
    std::vector<double> operator()(const std::vector<double>& v_fine,
                                    std::shared_ptr<Grid1D> fine_grid,
                                    std::shared_ptr<Grid1D> coarse_grid) const;
    
    // 设置边界类型
    void setBoundaryTypes(const std::string& lt, const std::string& rt) {
        left_type = lt;
        right_type = rt;
    }
    
private:
    std::string left_type = "dirichlet";
    std::string right_type = "dirichlet";
};

#endif