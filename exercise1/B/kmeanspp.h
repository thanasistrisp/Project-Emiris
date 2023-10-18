#pragma once

#include <vector>
#include "metrics.hpp"

#define dist euclidean_distance

std::vector<std::vector<double>> kmeanspp(std::vector<std::vector<double>> p, int k);