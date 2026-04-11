#ifndef PROLONGATOR_H
#define PROLONGATOR_H

#include <vector>
#include <memory>
#include "Grid1D.h"

class Prolongator {
public:
    // 线性插值延拓
    std::vector<double> operator()(const std::vector<double>& v_coarse,
                                    std::shared_ptr<Grid1D> coarse_grid,
                                    std::shared_ptr<Grid1D> fine_grid) const;
};

#endif