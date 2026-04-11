#include "Prolongator.h"

std::vector<double> Prolongator::operator()(const std::vector<double>& v_coarse,
                                              std::shared_ptr<Grid1D> coarse_grid,
                                              std::shared_ptr<Grid1D> fine_grid) const {
    int n_fine = fine_grid->getN();
    int n_coarse = coarse_grid->getN();
    std::vector<double> v_fine(n_fine + 1, 0.0);
    
    // 边界点
    v_fine[0] = v_coarse[0];
    v_fine[n_fine] = v_coarse[n_coarse];
    
    // 偶数点直接复制
    for (int j = 1; j <= n_coarse - 1; j++) {
        v_fine[2 * j] = v_coarse[j];
    }
    
    // 奇数点线性插值
    for (int j = 0; j <= n_coarse - 1; j++) {
        v_fine[2 * j + 1] = 0.5 * (v_coarse[j] + v_coarse[j + 1]);
    }
    
    return v_fine;
}