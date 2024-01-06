#pragma once

#include <vector>
#include <string>

// Returns the euclidean distance between two vectors, or -1 if an error occurs.
double euclidean_distance(const std::vector<double>&, const std::vector<double>&);

// Returns the squared euclidean distance between two vectors, or -1 if an error occurs.
double euclidean_distance_squared(const std::vector<double>&, const std::vector<double>&);

// Returns the lp-distance between two vectors, or -1 if an error occurs.
// Third argument is p.
double lp_metric(std::vector<double>&, std::vector<double>&, int);

// Returns the lp-metric for the given vector.
double lp_metric(std::vector<double>&, std::string);