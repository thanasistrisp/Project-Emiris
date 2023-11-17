# pragma once

#include "gnn.hpp"
#include "mrng.hpp"

void handle_ouput(MRNG &mrng, const std::vector<std::vector<double>> &dataset,
                  const std::vector<std::vector<double>> &queries, int N, std::ofstream &output);