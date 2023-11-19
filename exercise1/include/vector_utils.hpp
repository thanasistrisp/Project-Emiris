#pragma once

#include <vector>

std::vector<double> vector_addition(const std::vector<double>&, const std::vector<double>&);    // v1 + v2
std::vector<double> vector_subtraction(const std::vector<double>&, const std::vector<double>&); // v1 - v2
std::vector<double> vector_scalar_mult(const std::vector<double>&, double);                     // v * n (scalar)