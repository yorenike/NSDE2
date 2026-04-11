#ifndef RESTRICTOR_H
#define RESTRICTOR_H

#include <vector>
#include <memory>
#include "Grid1D.h"

class Restrictor {
public:
    // 全加权限制
    std::vector<double> operator()(const std::vector<double>& v_fine,
                                    std::shared_ptr<Grid1D> fine_grid,
                                    std::shared_ptr<Grid1D> coarse_grid) const;
};

#endif