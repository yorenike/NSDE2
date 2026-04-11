#ifndef VCYCLE_H
#define VCYCLE_H

#include <vector>
#include <memory>
#include "Grid1D.h"
#include "Restrictor.h"
#include "Prolongator.h"
#include "Smoother.h"

class VCycle {
private:
    Restrictor restrictor;
    Prolongator prolongator;
    Smoother smoother;
    
public:
    // 构造函数内联定义
    VCycle(const Restrictor& R = Restrictor(),
           const Prolongator& P = Prolongator(),
           const Smoother& S = Smoother())
        : restrictor(R), prolongator(P), smoother(S) {}
    
    std::vector<double> solve(std::vector<double> u,
                               const std::vector<double>& f,
                               std::shared_ptr<Grid1D> grid,
                               int nu1, int nu2) const;
};

#endif // VCYCLE_H