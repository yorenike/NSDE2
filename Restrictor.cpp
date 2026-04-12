#include "Restrictor.h"

std::vector<double> Restrictor::operator()(const std::vector<double>& v_fine,
                                             std::shared_ptr<Grid1D> fine_grid,
                                             std::shared_ptr<Grid1D> coarse_grid) const {
    int n_fine = fine_grid->getN();
    int n_coarse = coarse_grid->getN();
    std::vector<double> v_coarse(n_coarse + 1, 0.0);
    
    // ========== 左边界 ==========
    if (left_type == "dirichlet") {
        // Dirichlet: 直接复制（边界值已知）
        v_coarse[0] = v_fine[0];
    } else if (left_type == "neumann") {
        // Neumann: 对称平均（因为齐次 Neumann 条件，r_{-1} = r_1）
        // 对于残差，边界点应该用 (r0 + r1)/2
        v_coarse[0] = 0.5 * (v_fine[0] + v_fine[1]);
    }
    
    // ========== 右边界 ==========
    if (right_type == "dirichlet") {
        v_coarse[n_coarse] = v_fine[n_fine];
    } else if (right_type == "neumann") {
        // Neumann: 对称平均
        v_coarse[n_coarse] = 0.5 * (v_fine[n_fine - 1] + v_fine[n_fine]);
    }
    
    // ========== 内部点：全加权 ==========
    for (int j = 1; j <= n_coarse - 1; j++) {
        int i_fine = 2 * j;
        v_coarse[j] = (v_fine[i_fine - 1] + 2.0 * v_fine[i_fine] + v_fine[i_fine + 1]) / 4.0;
    }
    
    return v_coarse;
}