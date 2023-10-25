#pragma once

#include <tuple>
#include <vector>

std::tuple<std::vector<int>, std::vector<double>> brute_force(std::vector<std::vector<double>> dataset, std::vector<double> query, 
															  unsigned int N, double (*distance)(const std::vector<double>&, const std::vector<double>&));