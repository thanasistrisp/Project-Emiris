#pragma once

#include <vector>
#include <string>
#include "defines.hpp"

// Returns the euclidean distance between two vectors, or -1 if an error occurs.
double euclidean_distance(const std::vector<point>&, const std::vector<point>&);

// Returns the lp-distance between two vectors, or -1 if an error occurs.
// Third argument is p.
double lp_metric(std::vector<point>&, std::vector<point>&, int);

// Returns the lp-metric for the given vector.
double lp_metric(std::vector<point>&, std::string);