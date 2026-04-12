#ifndef VCYCLE_H
#define VCYCLE_H

#include <vector>
#include <memory>
#include <string>
#include "Grid1D.h"
#include "Restrictor.h"
#include "Prolongator.h"
#include "Smoother.h"

class VCycle {
private:
    Restrictor restrictor;
    Prolongator prolongator;
    Smoother smoother;
    std::string left_type;
    std::string right_type;
    
public:
    // 构造函数：接收边界类型
    VCycle(const Restrictor& R = Restrictor(),
           const Prolongator& P = Prolongator(),
           const Smoother& S = Smoother(),
           const std::string& lt = "dirichlet",
           const std::string& rt = "dirichlet")
        : restrictor(R), prolongator(P), smoother(S), left_type(lt), right_type(rt) {}
    
    std::vector<double> solve(std::vector<double> u,
                               const std::vector<double>& f,
                               std::shared_ptr<Grid1D> grid,
                               int nu1, int nu2) const;
};

#endif