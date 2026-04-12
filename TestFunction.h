#ifndef TEST_FUNCTION_1D_H
#define TEST_FUNCTION_1D_H

#include <string>
#include <functional>
#include <cmath>
#include <stdexcept>

class TestFunction1D {
private:
    std::string name;
    
    // 解析解 u(x)
    std::function<double(double)> u_func;
    
    // 右端项 f(x) = -u''(x)
    std::function<double(double)> f_func;
    
    // 一阶导数 u'(x)（用于 Neumann 边界）
    std::function<double(double)> du_func;
    
    // 初始化各个测试函数
    void initQuadratic();      // u(x) = x(1-x)
    void initTrigonometric();  // u(x) = sin²(πx)
    void initPolynomial();     // u(x) = x⁴(1-x)⁴
    void initExpSinX();        // u(x) = exp(x + sin x)
    
public:
    // 构造函数
    TestFunction1D(const std::string& name);
    
    // 获取解析解
    double exact(double x) const { return u_func(x); }
    
    // 获取右端项 f = -u''
    double f(double x) const { return f_func(x); }
    
    // 获取一阶导数（用于 Neumann 边界）
    double du(double x) const { return du_func(x); }
    
    // 获取测试函数名称
    std::string getName() const { return name; }
};

#endif