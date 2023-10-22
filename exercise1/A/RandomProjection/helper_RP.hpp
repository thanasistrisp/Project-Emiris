#pragma once

#include <vector>
#include <string>
#include <tuple>
#include <fstream>

#include "hypercube.hpp"

void handle_ouput(hypercube &cube, std::ofstream &output, const std::vector<std::vector<double>> &queries, double R, int N);