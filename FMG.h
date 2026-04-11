#ifndef FMG_H
#define FMG_H

#include <vector>
#include <memory>
#include "Grid1D.h"
#include "Restrictor.h"
#include "Prolongator.h"
#include "Smoother.h"
#include "VCycle.h"

class FMG {
private:
    Restrictor restrictor;
    Prolongator prolongator;
    Smoother smoother;
    VCycle vcycle;
    
public:
    FMG(const Restrictor& R = Restrictor(),
        const Prolongator& P = Prolongator(),
        const Smoother& S = Smoother(),
        const VCycle& V = VCycle())
        : restrictor(R), prolongator(P), smoother(S), vcycle(V) {}
    
    std::vector<double> solve(const std::vector<double>& f,
                               std::shared_ptr<Grid1D> grid,
                               int nu1, int nu2) const;
};

#endif