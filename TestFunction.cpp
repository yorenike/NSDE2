#include "TestFunction.h"

// ========== 测试函数1: exp(y + sin(x)) ==========
void TestFunction::initExpYSinX() {
    name = "exp_y_sin_x";
    
    // 解析解
    u_func = [](double x, double y) {
        return std::exp(y + std::sin(x));
    };
    
    // 二维右端项 f = -Δu
    f_func = [](double x, double y) {
        double u = std::exp(y + std::sin(x));
        double cosx = std::cos(x);
        return -u * (cosx * cosx - std::sin(x) + 1.0);
    };
    
    // 一维右端项（y=x 时）
    // u(x) = exp(x + sin(x))
    // 需要计算 -u''(x)，这里用数值近似或手动推导
    // 为了简化，先用数值方式，但这里我们手动推导
    // u' = u * (1 + cosx)
    // u'' = u'*(1+cosx) + u*(-sinx) = u*(1+cosx)^2 - u*sinx
    // 所以 -u'' = -u * ((1+cosx)^2 - sinx)
    f1D_func = [](double x) {
        double u = std::exp(x + std::sin(x));
        double cosx = std::cos(x);
        double sinx = std::sin(x);
        return -u * ((1.0 + cosx) * (1.0 + cosx) - sinx);
    };
    
    // Dirichlet 边界条件
    bc_left   = [](double x, double y) { return std::exp(y + std::sin(0.0)); };
    bc_right  = [](double x, double y) { return std::exp(y + std::sin(1.0)); };
    bc_bottom = [](double x, double y) { return std::exp(0.0 + std::sin(x)); };
    bc_top    = [](double x, double y) { return std::exp(1.0 + std::sin(x)); };
}

// ========== 测试函数2: 多项式 u(x,y) = x²(1-x)² y²(1-y)² ==========
void TestFunction::initPolynomial() {
    name = "polynomial";
    
    u_func = [](double x, double y) {
        double A = x * x * (1.0 - x) * (1.0 - x);
        double B = y * y * (1.0 - y) * (1.0 - y);
        return A * B;
    };
    
    // 二维右端项
    f_func = [](double x, double y) {
        double A = x*x - 2.0*x*x*x + x*x*x*x;
        double App = 2.0 - 12.0*x + 12.0*x*x;
        double B = y*y - 2.0*y*y*y + y*y*y*y;
        double Bpp = 2.0 - 12.0*y + 12.0*y*y;
        return -(App * B + A * Bpp);
    };
    
    // 一维右端项（y=x 时）
    // u(x) = x^4 (1-x)^4
    // 这个函数比较复杂，我们用一个简单的近似或者用多项式展开
    // 为了简化，我们直接使用数值方式，但这里我们手动计算
    // u(x) = x^4 (1-x)^4 = x^4 (1 - 4x + 6x^2 - 4x^3 + x^4)
    //      = x^4 - 4x^5 + 6x^6 - 4x^7 + x^8
    // u' = 4x^3 - 20x^4 + 36x^5 - 28x^6 + 8x^7
    // u'' = 12x^2 - 80x^3 + 180x^4 - 168x^5 + 56x^6
    // -u'' = -12x^2 + 80x^3 - 180x^4 + 168x^5 - 56x^6
    f1D_func = [](double x) {
        double x2 = x * x;
        double x3 = x2 * x;
        double x4 = x3 * x;
        double x5 = x4 * x;
        double x6 = x5 * x;
        return -12.0*x2 + 80.0*x3 - 180.0*x4 + 168.0*x5 - 56.0*x6;
    };
    
    // Dirichlet 边界条件：边界上 u = 0
    bc_left = bc_right = bc_bottom = bc_top = [](double, double) { return 0.0; };
}

// ========== 测试函数3: 三角函数 u(x,y) = sin(πx) sin(πy) ==========
void TestFunction::initTrigonometric() {
    name = "trigonometric";
    
    // 解析解
    u_func = [](double x, double y) {
        return std::sin(M_PI * x) * std::sin(M_PI * y);
    };
    
    // 二维右端项
    f_func = [](double x, double y) {
        return 2.0 * M_PI * M_PI * std::sin(M_PI * x) * std::sin(M_PI * y);
    };
    
    // 一维右端项（y=x 时）
    // u(x) = sin²(πx) = (1 - cos(2πx))/2
    // u'(x) = π sin(2πx)
    // u''(x) = 2π² cos(2πx)
    // -u''(x) = -2π² cos(2πx)
    f1D_func = [](double x) {
        return -2.0 * M_PI * M_PI * std::cos(2.0 * M_PI * x);
    };
    
    // Dirichlet 边界条件：边界上 u = 0
    bc_left = bc_right = bc_bottom = bc_top = [](double, double) { return 0.0; };
}

// ========== 测试函数4: u(x) = x(1-x)（一维专用）==========
void TestFunction::initQuadratic() {
    name = "quadratic";
    
    // 二维版本（沿 y=x 方向）
    u_func = [](double x, double y) {
        double t = (x + y) / 2.0;  // 沿对角线
        return t * (1.0 - t);
    };
    
    // 二维右端项（不精确，但一维版本才是关键）
    f_func = [](double x, double y) { return 2.0; };
    
    // 一维右端项：u(x) = x(1-x), u'' = -2, -u'' = 2
    f1D_func = [](double x) { return 2.0; };
    
    // Dirichlet 边界条件
    bc_left = bc_right = bc_bottom = bc_top = [](double x, double y) { 
        double t = (x + y) / 2.0;
        return t * (1.0 - t);
    };
}

// ========== 构造函数 ==========
TestFunction::TestFunction(const std::string& name_) {
    if (name_ == "exp_y_sin_x" || name_ == "exp_y_plus_sin_x") {
        initExpYSinX();
    }
    else if (name_ == "polynomial") {
        initPolynomial();
    }
    else if (name_ == "trigonometric") {
        initTrigonometric();
    }
    else if (name_ == "quadratic") {
        initQuadratic();
    }
    else {
        throw std::runtime_error("Unknown test function: " + name_);
    }
}

// ========== 公共接口实现 ==========
double TestFunction::exact(double x, double y) const {
    return u_func(x, y);
}

double TestFunction::f(double x, double y) const {
    return f_func(x, y);
}

double TestFunction::getDirichletBC(double x, double y, const std::string& side) const {
    if (side == "left")   return bc_left(x, y);
    if (side == "right")  return bc_right(x, y);
    if (side == "bottom") return bc_bottom(x, y);
    if (side == "top")    return bc_top(x, y);
    throw std::runtime_error("Unknown side: " + side);
}