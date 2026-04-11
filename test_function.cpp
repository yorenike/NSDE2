#include "TestFunction.h"
#include <iostream>

int main() {
    TestFunction tf("exp_y_sin_x");
    
    // 测试二维
    double x = 0.5, y = 0.3;
    std::cout << "u(0.5, 0.3) = " << tf.exact(x, y) << std::endl;
    std::cout << "f(0.5, 0.3) = " << tf.f(x, y) << std::endl;
    
    // 测试一维（y = x）
    double t = 0.5;
    std::cout << "u(t, t) = " << tf.exact1D(t) << std::endl;
    std::cout << "f(t, t) = " << tf.f1D(t) << std::endl;
    
    return 0;
}