# pragma once

#include <tuple>

#include "ApproximateKNNGraph.hpp"
#include "mrng.hpp"

void handle_ouput(void *structure, const std::vector<std::vector<double>> &dataset,
                  const std::vector<std::vector<double>> &queries, std::ofstream &output,
                  std::vector<int> &params);