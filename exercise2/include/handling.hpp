# pragma once

#include "gnn.hpp"

void handle_ouput(GNN &gnn, const std::vector<std::vector<double>> &dataset,
                  const std::vector<std::vector<double>> &queries, int Ν, std::ofstream &output);