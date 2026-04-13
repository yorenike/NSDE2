#ifndef RESTRICTOR_H
#define RESTRICTOR_H

#include <vector>
#include <memory>
#include <string>
#include "Grid1D.h"

class Restrictor {
private:
    std::string left_type = "dirichlet";
    std::string right_type = "dirichlet";
    std::string restrict_type = "full_weighting";  // "full_weighting" 或 "injection"
    
public:
    Restrictor() = default;
    
    Restrictor(const std::string& lt, const std::string& rt, const std::string& rt_type = "full_weighting")
        : left_type(lt), right_type(rt), restrict_type(rt_type) {}
    
    void setBoundaryTypes(const std::string& lt, const std::string& rt) {
        left_type = lt;
        right_type = rt;
    }
    
    void setRestrictType(const std::string& type) {
        restrict_type = type;
    }
    
    std::vector<double> operator()(const std::vector<double>& v_fine,
                                    std::shared_ptr<Grid1D> fine_grid,
                                    std::shared_ptr<Grid1D> coarse_grid) const;
};

#endif