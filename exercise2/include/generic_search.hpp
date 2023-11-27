#pragma once

#include <vector>
#include <tuple>

#include "directed_graph.hpp"

std::tuple<std::tuple<std::vector<int>, std::vector<double>>, std::vector<int>> generic_search_on_graph(const DirectedGraph &,
                                        const std::vector<std::vector<double>>&, int, const std::vector<double>&, int, unsigned int,
                                        double (*distance)(const std::vector<double>&, const std::vector<double>&));