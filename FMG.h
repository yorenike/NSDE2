#ifndef FMG_H
#define FMG_H

#include <vector>
#include <memory>
#include <string>
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
    std::string left_type;
    std::string right_type;
    
public:
    // 构造函数：接收边界类型
    FMG(const Restrictor& R = Restrictor(),
        const Prolongator& P = Prolongator(),
        const Smoother& S = Smoother(),
        const VCycle& V = VCycle(),
        const std::string& lt = "dirichlet",
        const std::string& rt = "dirichlet")
        : restrictor(R), prolongator(P), smoother(S), vcycle(V), left_type(lt), right_type(rt) {}
    
    std::vector<double> solve(const std::vector<double>& f,
                               std::shared_ptr<Grid1D> grid,
                               int nu1, int nu2) const;
};

#endif