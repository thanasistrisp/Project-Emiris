#pragma once

#include <vector>
#include <tuple>

#include "directed_graph.hpp"

std::tuple<std::vector<int>, std::vector<double>> generic_search_on_graph(DirectedGraph &, std::vector<std::vector<double>>&,
                                                                          int, std::vector<double>&, int, unsigned int,
                                                                          double (*distance)(const std::vector<double>&, const std::vector<double>&));