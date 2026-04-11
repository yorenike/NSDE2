#include "Restrictor.h"

std::vector<double> Restrictor::operator()(const std::vector<double>& v_fine,
                                             std::shared_ptr<Grid1D> fine_grid,
                                             std::shared_ptr<Grid1D> coarse_grid) const {
    int n_fine = fine_grid->getN();
    int n_coarse = coarse_grid->getN();
    std::vector<double> v_coarse(n_coarse + 1, 0.0);
    
    // 边界点直接复制
    v_coarse[0] = v_fine[0];
    v_coarse[n_coarse] = v_fine[n_fine];
    
    // 内部点：全加权
    for (int j = 1; j <= n_coarse - 1; j++) {
        int i_fine = 2 * j;
        v_coarse[j] = (v_fine[i_fine - 1] + 2.0 * v_fine[i_fine] + v_fine[i_fine + 1]) / 4.0;
    }
    
    return v_coarse;
}