#pragma once

#include <tuple>
#include <vector>

#include "lp_metric.hpp"

// Returns the indices of the k-exact nearest neighbours (k-NN) of the given query q
// and their distances to the query based on the given distance function.
std::tuple<std::vector<int>, std::vector<double>> brute_force(std::vector<std::vector<double>> dataset, std::vector<double> query, 
															  unsigned int N, double (*distance)(const std::vector<double>&, const std::vector<double>&) = euclidean_distance);