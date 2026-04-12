#ifndef GRID1D_H
#define GRID1D_H

#include <vector>
#include <memory>
#include <iostream>

class Grid1D : public std::enable_shared_from_this<Grid1D> {
private:
    int n;                // 区间数
    double h;             // 步长
    double xmin, xmax;
    std::vector<double> x_coords;
    std::shared_ptr<Grid1D> coarse_grid;
    
public:
    Grid1D(int n_, double xmin_ = 0.0, double xmax_ = 1.0);
    Grid1D(std::shared_ptr<Grid1D> fine_grid);
    
    // 基本获取方法
    int getN() const { return n; }
    double getH() const { return h; }
    double getX(int i) const { return x_coords[i]; }
    double getXmin() const { return xmin; }
    double getXmax() const { return xmax; }
    int getNumTotal() const { return n + 1; }
    int getNumInterior() const { return n - 1; }
    
    // 边界判断
    bool isInterior(int i) const { return i > 0 && i < n; }
    bool isBoundary(int i) const { return i == 0 || i == n; }
    
    // 多重网格相关
    std::shared_ptr<Grid1D> getCoarseGrid();
    bool isCoarsest() const { return n <= 8; }
};

#endif