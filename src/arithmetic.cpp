#include "arithmetic.h"
#include <stdexcept>

namespace arithmetic {

double add(double lhs, double rhs) {
    return lhs + rhs;
}

double subtract(double lhs, double rhs) {
    return lhs - rhs;
}

double multiply(double lhs, double rhs) {
    return lhs * rhs;
}

double divide(double lhs, double rhs) {
    if (rhs == 0.0) {
        throw std::invalid_argument("division by zero");
    }
    return lhs / rhs;
}

}


