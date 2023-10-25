#pragma once

#include <vector>
#include <string>

#include "lsh.hpp"

void handle_ouput(LSH &cube, const std::vector<std::vector<double>> &dataset,
                  const std::vector<std::vector<double>> &queries, int n, double r, std::ofstream &output);