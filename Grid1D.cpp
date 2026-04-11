#include "Grid1D.h"
#include <cmath>
#include <stdexcept>

Grid1D::Grid1D(int n_, double xmin_, double xmax_)
    : n(n_), xmin(xmin_), xmax(xmax_) {
    
    if (n_ <= 0) {
        throw std::runtime_error("Grid1D: n must be positive");
    }
    
    h = (xmax - xmin) / n;
    
    x_coords.resize(n + 1);
    for (int i = 0; i <= n; i++) {
        x_coords[i] = xmin + i * h;
    }
}

Grid1D::Grid1D(std::shared_ptr<Grid1D> fine_grid)
    : n(fine_grid->n / 2), 
      xmin(fine_grid->xmin), 
      xmax(fine_grid->xmax) {
    
    if (n < 1) {
        throw std::runtime_error("Grid1D: coarse grid too small");
    }
    
    h = (xmax - xmin) / n;
    
    x_coords.resize(n + 1);
    for (int i = 0; i <= n; i++) {
        x_coords[i] = xmin + i * h;
    }
}

std::shared_ptr<Grid1D> Grid1D::getCoarseGrid() {
    if (!coarse_grid && !isCoarsest()) {
        coarse_grid = std::make_shared<Grid1D>(shared_from_this());
    }
    return coarse_grid;
}