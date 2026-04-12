#include "TestFunction.h"

// ========== u(x) = x(1-x) ==========
void TestFunction1D::initQuadratic() {
    name = "quadratic";
    
    // u(x) = x(1-x) = x - x²
    u_func = [](double x) {
        return x * (1.0 - x);
    };
    
    // u'(x) = 1 - 2x
    du_func = [](double x) {
        return 1.0 - 2.0 * x;
    };
    
    // u''(x) = -2, 所以 -u'' = 2
    f_func = [](double x) {
        return 2.0;
    };
}

// ========== u(x) = sin²(πx) ==========
void TestFunction1D::initTrigonometric() {
    name = "trigonometric";
    
    // u(x) = sin²(πx) = (1 - cos(2πx))/2
    u_func = [](double x) {
        double s = std::sin(M_PI * x);
        return s * s;
    };
    
    // u'(x) = π sin(2πx)
    du_func = [](double x) {
        return M_PI * std::sin(2.0 * M_PI * x);
    };
    
    // u''(x) = 2π² cos(2πx), 所以 -u'' = -2π² cos(2πx)
    f_func = [](double x) {
        return -2.0 * M_PI * M_PI * std::cos(2.0 * M_PI * x);
    };
}

// ========== u(x) = x⁴(1-x)⁴ ==========
void TestFunction1D::initPolynomial() {
    name = "polynomial";
    
    // u(x) = x⁴(1-x)⁴ = x⁴ - 4x⁵ + 6x⁶ - 4x⁷ + x⁸
    u_func = [](double x) {
        double x2 = x * x;
        double x4 = x2 * x2;
        double x5 = x4 * x;
        double x6 = x5 * x;
        double x7 = x6 * x;
        double x8 = x7 * x;
        return x4 - 4.0*x5 + 6.0*x6 - 4.0*x7 + x8;
    };
    
    // u'(x) = 4x³ - 20x⁴ + 36x⁵ - 28x⁶ + 8x⁷
    du_func = [](double x) {
        double x2 = x * x;
        double x3 = x2 * x;
        double x4 = x3 * x;
        double x5 = x4 * x;
        double x6 = x5 * x;
        double x7 = x6 * x;
        return 4.0*x3 - 20.0*x4 + 36.0*x5 - 28.0*x6 + 8.0*x7;
    };
    
    // -u'' = -12x² + 80x³ - 180x⁴ + 168x⁵ - 56x⁶
    f_func = [](double x) {
        double x2 = x * x;
        double x3 = x2 * x;
        double x4 = x3 * x;
        double x5 = x4 * x;
        double x6 = x5 * x;
        return -12.0*x2 + 80.0*x3 - 180.0*x4 + 168.0*x5 - 56.0*x6;
    };
}

// ========== u(x) = exp(x + sin x) ==========
void TestFunction1D::initExpSinX() {
    name = "exp_y_sin_x";
    
    // u(x) = exp(x + sin x)
    u_func = [](double x) {
        return std::exp(x + std::sin(x));
    };
    
    // u'(x) = u(x) * (1 + cos x)
    du_func = [](double x) {
        double u = std::exp(x + std::sin(x));
        return u * (1.0 + std::cos(x));
    };
    
    // u''(x) = u(x) * ((1+cos x)² - sin x)
    // -u''(x) = -u(x) * ((1+cos x)² - sin x)
    f_func = [](double x) {
        double u = std::exp(x + std::sin(x));
        double cosx = std::cos(x);
        double sinx = std::sin(x);
        return -u * ((1.0 + cosx) * (1.0 + cosx) - sinx);
    };
}

// ========== 构造函数 ==========
TestFunction1D::TestFunction1D(const std::string& name_) {
    if (name_ == "quadratic") {
        initQuadratic();
    }
    else if (name_ == "trigonometric") {
        initTrigonometric();
    }
    else if (name_ == "polynomial") {
        initPolynomial();
    }
    else if (name_ == "exp_y_sin_x") {
        initExpSinX();
    }
    else {
        throw std::runtime_error("Unknown 1D test function: " + name_);
    }
}