#ifndef TEST_FUNCTION_H
#define TEST_FUNCTION_H

#include <string>
#include <functional>
#include <cmath>
#include <stdexcept>

class TestFunction {
private:
    std::string name;
    
    // 解析解 u(x,y)
    std::function<double(double, double)> u_func;
    
    // 二维右端项 f(x,y) = -Δu
    std::function<double(double, double)> f_func;
    
    // 一维右端项 f1D(x)（用于 y=x 投影时的一维 Poisson 方程）
    std::function<double(double)> f1D_func;
    
    // Dirichlet 边界条件
    std::function<double(double, double)> bc_left;
    std::function<double(double, double)> bc_right;
    std::function<double(double, double)> bc_bottom;
    std::function<double(double, double)> bc_top;
    
    // 初始化各个测试函数
    void initExpYSinX();
    void initPolynomial();
    void initTrigonometric();
    void initQuadratic();  // 新增：u(x) = x(1-x) 测试函数
    
public:
    // 构造函数：根据名称选择测试函数
    TestFunction(const std::string& name);
    
    // 获取解析解（二维）
    double exact(double x, double y) const;
    
    // 获取右端项（二维）
    double f(double x, double y) const;
    
    // 获取一维右端项（用于 y=x 投影）
    double f1D(double x) const { return f1D_func(x); }
    
    // 获取 Dirichlet 边界条件值
    double getDirichletBC(double x, double y, const std::string& side) const;
    
    // 获取测试函数名称
    std::string getName() const { return name; }
    
    // 判断边界值是否来自测试函数
    static bool isFromTestFunction(const std::string& value) {
        return value == "from_test_function";
    }
    
    // 一维版本（用于测试函数在 y = x 上的投影）
    double exact1D(double x) const { return exact(x, x); }
    double bcLeft1D(double x) const { return getDirichletBC(0, x, "left"); }
    double bcRight1D(double x) const { return getDirichletBC(1, x, "right"); }
};

#endif