#pragma once

#include <vector>
#include "metrics.hpp"

// vector<double> query(vector<vector<double>> p, vector<double> q, int k, int M, int probes, double (*distance)(vector<double>, vector<double>) = euclidean_distance)
std::vector<double> query(std::vector<std::vector<double>> p, std::vector<double> q, int k, int M, int probes, 
						  double (*distance)(std::vector<double>, std::vector<double> distance) = euclidean_distance);