#include "Prolongator.h"

std::vector<double> Prolongator::operator()(const std::vector<double>& v_coarse,
                                              std::shared_ptr<Grid1D> coarse_grid,
                                              std::shared_ptr<Grid1D> fine_grid) const {
    int n_fine = fine_grid->getN();
    int n_coarse = coarse_grid->getN();
    std::vector<double> v_fine(n_fine + 1, 0.0);
    
    // 边界点直接复制
    v_fine[0] = v_coarse[0];
    v_fine[n_fine] = v_coarse[n_coarse];
    
    if (interp_type == "linear") {
        // ========== 线性插值 ==========
        for (int j = 1; j <= n_coarse - 1; j++) {
            v_fine[2 * j] = v_coarse[j];
        }
        for (int j = 0; j <= n_coarse - 1; j++) {
            v_fine[2 * j + 1] = 0.5 * (v_coarse[j] + v_coarse[j + 1]);
        }
        
    } else if (interp_type == "quadratic") {
        // ========== 二次插值 ==========
        
        // 偶数点直接复制
        for (int j = 0; j <= n_coarse; j++) {
            v_fine[2 * j] = v_coarse[j];
        }
        
        // 奇数点二次插值
        for (int j = 0; j <= n_coarse - 1; j++) {
            int idx = 2 * j + 1;  // 细网格奇数点索引
            
            if (j == 0) {
                // 第一个奇数点 (x_1)：左边只有一个点，用左边一个点 + 右边两个点
                // 使用三点公式：v1 = 3/4 * v0 + 1/4 * v1_coarse? 不对
                // 正确公式：用 v0, v1, v2 三点二次插值，求 x=0.5h 处的值
                // 设 x0=0, x1=h, x2=2h，对应值 v0, v1, v2
                // 在 x = h/2 处插值：v = 3/8*v0 + 3/4*v1 - 1/8*v2
                v_fine[idx] = 0.375 * v_coarse[0] + 0.75 * v_coarse[1] - 0.125 * v_coarse[2];
                
            } else if (j == n_coarse - 1) {
                // 最后一个奇数点 (x_{n-1})：右边只有一个点，用左边两个点 + 右边一个点
                // 使用三点公式：用 v_{n-3}, v_{n-2}, v_{n-1} 三点二次插值
                // 求 x = (n-1.5)h 处的值
                // v = -1/8*v_{n-3} + 3/4*v_{n-2} + 3/8*v_{n-1}
                v_fine[idx] = -0.125 * v_coarse[j - 1] + 0.75 * v_coarse[j] + 0.375 * v_coarse[j + 1];
                
            } else {
                // 内部奇数点：用四个点进行二次插值（四点公式）
                // v_{2j+1} = 9/16*v_j + 9/16*v_{j+1} - 1/16*v_{j-1} - 1/16*v_{j+2}
                v_fine[idx] = (9.0/16.0) * v_coarse[j] 
                            + (9.0/16.0) * v_coarse[j + 1] 
                            - (1.0/16.0) * v_coarse[j - 1] 
                            - (1.0/16.0) * v_coarse[j + 2];
            }
        }
    }
    
    return v_fine;
}