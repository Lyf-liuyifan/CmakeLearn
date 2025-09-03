#pragma once

namespace arithmetic {

double add(double lhs, double rhs);

double subtract(double lhs, double rhs);

double multiply(double lhs, double rhs);

// Throws std::invalid_argument when rhs is 0
double divide(double lhs, double rhs);

}


