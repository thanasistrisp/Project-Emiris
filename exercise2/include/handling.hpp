# pragma once

#include <tuple>

#include "approximate_knn_graph.hpp"
#include "mrng.hpp"

// Writes the results of the queries to output file in the required format.
void handle_ouput(void *structure, const std::vector<std::vector<double>> &dataset,
                  const std::vector<std::vector<double>> &queries, std::ofstream &output,
                  std::vector<int> &params);