#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <map>
#include <vector>

class PolynomialSolver {
private:
    std::map<int, double> coefficients;
    
    // Helper functions
    std::vector<std::string> split(const std::string& str, char delimiter);
    std::string trim(const std::string& str);
    void parseEquation(const std::string& equation);
    void parseSide(const std::string& side, bool isRight);
    void parseTerm(const std::string& term, bool isRight);
    void displayReducedForm();
    int getDegree();
    void solveDegree0();
    void solveDegree1();
    void solveDegree2();
    double sqrt(double x);
    
public:
    PolynomialSolver();
    void solve(const std::string& equation);
};

#endif