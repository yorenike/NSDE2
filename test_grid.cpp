#include "Grid1D.h"
#include <iostream>

int main() {
    // 创建细网格
    auto fine = std::make_shared<Grid1D>(8, 0.0, 1.0);
    
    std::cout << "Fine grid:" << std::endl;
    std::cout << "  n = " << fine->getN() << std::endl;
    std::cout << "  h = " << fine->getH() << std::endl;
    std::cout << "  numTotal = " << fine->getNumTotal() << std::endl;
    std::cout << "  numInterior = " << fine->getNumInterior() << std::endl;
    std::cout << "  x coordinates: ";
    for (int i = 0; i <= fine->getN(); i++) {
        std::cout << fine->getX(i) << " ";
    }
    std::cout << std::endl;
    
    // 创建粗网格
    auto coarse = fine->getCoarseGrid();
    
    std::cout << "\nCoarse grid:" << std::endl;
    std::cout << "  n = " << coarse->getN() << std::endl;
    std::cout << "  h = " << coarse->getH() << std::endl;
    std::cout << "  numTotal = " << coarse->getNumTotal() << std::endl;
    std::cout << "  x coordinates: ";
    for (int i = 0; i <= coarse->getN(); i++) {
        std::cout << coarse->getX(i) << " ";
    }
    std::cout << std::endl;
    
    return 0;
}