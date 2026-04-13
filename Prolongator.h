#ifndef PROLONGATOR_H
#define PROLONGATOR_H

#include <vector>
#include <memory>
#include <string>
#include "Grid1D.h"

class Prolongator {
private:
    std::string interp_type = "linear";  // "linear" 或 "quadratic"
    
public:
    Prolongator() = default;
    
    Prolongator(const std::string& type) : interp_type(type) {}
    
    void setInterpType(const std::string& type) {
        interp_type = type;
    }
    
    std::vector<double> operator()(const std::vector<double>& v_coarse,
                                    std::shared_ptr<Grid1D> coarse_grid,
                                    std::shared_ptr<Grid1D> fine_grid) const;
};

#endif