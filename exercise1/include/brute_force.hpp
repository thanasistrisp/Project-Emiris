#pragma once

#include <tuple>
#include <vector>
#include "defines.hpp"

// Returns the indices of the k-exact nearest neighbours (k-NN) of the given query q
// and their distances to the query based on the given distance function.
std::tuple<std::vector<int>, std::vector<double>> brute_force(std::vector<std::vector<point>> dataset, std::vector<point> query, 
															  unsigned int N, double (*distance)(const std::vector<point>&, const std::vector<point>&));