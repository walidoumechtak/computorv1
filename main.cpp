#include "utils.hpp"
#include <iostream>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " \"equation\"" << std::endl;
        return 1;
    }
    
    try {
        std::string equation = argv[1];
        PolynomialSolver solver;
        solver.solve(equation);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}