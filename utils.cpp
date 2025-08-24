#include "utils.hpp"
#include <iostream>
#include <sstream>
#include <regex>
#include <stdexcept>
#include <iomanip>
#include <cmath>

PolynomialSolver::PolynomialSolver() {}

std::vector<std::string> PolynomialSolver::split(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string token;
    
    while (std::getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

std::string PolynomialSolver::trim(const std::string& str) {
    size_t start = str.find_first_not_of(" \t");
    if (start == std::string::npos) return "";
    size_t end = str.find_last_not_of(" \t");
    return str.substr(start, end - start + 1);
}

void PolynomialSolver::parseEquation(const std::string& equation) {
    coefficients.clear();
    
    // Split by '='
    std::vector<std::string> sides = split(equation, '=');
    if (sides.size() != 2) {
        throw std::runtime_error("Invalid equation format");
    }
    
    parseSide(sides[0], false); // Left side
    parseSide(sides[1], true);  // Right side (subtract from left)
}

void PolynomialSolver::parseSide(const std::string& side, bool isRight) {
    std::string cleanSide = trim(side);
    
    // Replace spaces around + and - to make parsing easier
    std::string processed = "";
    for (size_t i = 0; i < cleanSide.length(); i++) {
        char c = cleanSide[i];
        if (c == '+' || c == '-') {
            if (i > 0) processed += " ";
            processed += c;
            processed += " ";
        } else {
            processed += c;
        }
    }
    
    // Split by spaces to get individual terms
    std::istringstream iss(processed);
    std::string term;
    std::string currentTerm = "";
    
    while (iss >> term) {
        if (term == "+" || term == "-") {
            if (!currentTerm.empty()) {
                parseTerm(currentTerm, isRight);
                currentTerm = "";
            }
            if (term == "-") {
                currentTerm = "-";
            }
        } else {
            currentTerm += term;
        }
    }
    
    if (!currentTerm.empty()) {
        parseTerm(currentTerm, isRight);
    }
}

void PolynomialSolver::parseTerm(const std::string& term, bool isRight) {
    std::string cleanTerm = trim(term);
    if (cleanTerm.empty()) return;
    
    // Regular expression to match terms like: coefficient * X^power
    std::regex termRegex(R"(([+-]?\s*\d*\.?\d*)\s*\*?\s*X\^?(\d+))");
    std::regex constantRegex(R"([+-]?\s*\d+\.?\d*)");
    std::smatch match;
    
    double coefficient = 0;
    int power = 0;
    
    if (std::regex_match(cleanTerm, match, termRegex)) {
        // Extract coefficient
        std::string coeffStr = match[1].str();
        coeffStr = trim(coeffStr);
        
        if (coeffStr.empty() || coeffStr == "+") {
            coefficient = 1;
        } else if (coeffStr == "-") {
            coefficient = -1;
        } else {
            coefficient = std::stod(coeffStr);
        }
        
        // Extract power
        power = std::stoi(match[2].str());
    } else if (std::regex_match(cleanTerm, match, constantRegex)) {
        // It's a constant term (X^0)
        coefficient = std::stod(cleanTerm);
        power = 0;
    } else {
        throw std::runtime_error("Invalid term format: " + cleanTerm);
    }
    
    // If it's on the right side, subtract from left side
    if (isRight) {
        coefficient = -coefficient;
    }
    
    coefficients[power] += coefficient;
}

void PolynomialSolver::displayReducedForm() {
    std::cout << "Reduced form: ";
    
    bool first = true;
    bool hasNonZero = false;
    
    // Find the highest degree
    int maxDegree = 0;
    for (const auto& pair : coefficients) {
        if (pair.second != 0 && pair.first > maxDegree) {
            maxDegree = pair.first;
        }
    }
    
    // Display terms in order from degree 0 to max degree
    for (int i = 0; i <= maxDegree; i++) {
        double coeff = coefficients[i];
        
        if (coeff != 0) {
            hasNonZero = true;
            
            if (!first) {
                if (coeff > 0) {
                    std::cout << " + ";
                } else {
                    std::cout << " - ";
                    coeff = -coeff;
                }
            } else {
                if (coeff < 0) {
                    std::cout << "-";
                    coeff = -coeff;
                }
            }
            
            std::cout << coeff << " * X^" << i;
            first = false;
        }
    }
    
    if (!hasNonZero) {
        std::cout << "0 * X^0";
    }
    
    std::cout << " = 0" << std::endl;
}

int PolynomialSolver::getDegree() {
    int degree = 0;
    for (const auto& pair : coefficients) {
        if (pair.second != 0 && pair.first > degree) {
            degree = pair.first;
        }
    }
    return degree;
}

double PolynomialSolver::sqrt(double x) {
    if (x < 0) return -1; // Invalid for real numbers
    if (x == 0) return 0;
    
    double result = x;
    double epsilon = 1e-10;
    
    while (true) {
        double newResult = 0.5 * (result + x / result);
        if (std::abs(newResult - result) < epsilon) {
            break;
        }
        result = newResult;
    }
    
    return result;
}

void PolynomialSolver::solveDegree0() {
    double a = coefficients[0];
    
    if (a == 0) {
        std::cout << "Any real number is a solution." << std::endl;
    } else {
        std::cout << "No solution." << std::endl;
    }
}

void PolynomialSolver::solveDegree1() {
    double a = coefficients[1];
    double b = coefficients[0];
    
    if (a == 0) {
        solveDegree0();
        return;
    }
    
    double solution = -b / a;
    std::cout << "The solution is:" << std::endl;
    std::cout << solution << std::endl;
}

void PolynomialSolver::solveDegree2() {
    double a = coefficients[2];
    double b = coefficients[1];
    double c = coefficients[0];
    
    if (a == 0) {
        solveDegree1();
        return;
    }
    
    double discriminant = b * b - 4 * a * c;
    
    if (discriminant > 0) {
        std::cout << "Discriminant is strictly positive, the two solutions are:" << std::endl;
        double sqrtD = sqrt(discriminant);
        double sol1 = (-b + sqrtD) / (2 * a);
        double sol2 = (-b - sqrtD) / (2 * a);
        std::cout << sol1 << std::endl;
        std::cout << sol2 << std::endl;
    } else if (discriminant == 0) {
        std::cout << "Discriminant is zero, the solution is:" << std::endl;
        double sol = -b / (2 * a);
        std::cout << sol << std::endl;
    } else {
        std::cout << "Discriminant is strictly negative, the two complex solutions are:" << std::endl;
        double realPart = -b / (2 * a);
        double imaginaryPart = sqrt(-discriminant) / (2 * a);
        
        // Display as fractions when possible
        if (a == 1) {
            std::cout << realPart << " + " << imaginaryPart << "i" << std::endl;
            std::cout << realPart << " - " << imaginaryPart << "i" << std::endl;
        } else {
            // Try to simplify fractions
            int bInt = static_cast<int>(b);
            int aInt = static_cast<int>(a);
            double sqrtDiscAbs = sqrt(-discriminant);
            int sqrtDiscInt = static_cast<int>(sqrtDiscAbs);
            
            if (bInt == b && aInt == a && sqrtDiscInt == sqrtDiscAbs) {
                // Display as fractions
                std::cout << "-" << bInt << "/" << (2 * aInt) << " + " << sqrtDiscInt << "i/" << (2 * aInt) << std::endl;
                std::cout << "-" << bInt << "/" << (2 * aInt) << " - " << sqrtDiscInt << "i/" << (2 * aInt) << std::endl;
            } else {
                std::cout << realPart << " + " << imaginaryPart << "i" << std::endl;
                std::cout << realPart << " - " << imaginaryPart << "i" << std::endl;
            }
        }
    }
}

void PolynomialSolver::solve(const std::string& equation) {
    parseEquation(equation);
    displayReducedForm();
    
    int degree = getDegree();
    std::cout << "Polynomial degree: " << degree << std::endl;
    
    if (degree > 2) {
        std::cout << "The polynomial degree is strictly greater than 2, I can't solve." << std::endl;
        return;
    }
    
    switch (degree) {
        case 0:
            solveDegree0();
            break;
        case 1:
            solveDegree1();
            break;
        case 2:
            solveDegree2();
            break;
        default:
            throw std::runtime_error("Unexpected degree");
    }
}