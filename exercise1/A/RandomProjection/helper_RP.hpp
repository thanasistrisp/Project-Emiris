#pragma once

#include <vector>
#include <string>
#include <tuple>
#include <fstream>

#include "hypercube.hpp"

// Writes the results of the queries to output file in the required format.
void handle_ouput(hypercube &cube, std::ofstream &output, const std::vector<std::vector<point>> &queries, double R, int N);