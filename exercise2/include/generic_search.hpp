#pragma once

#include <vector>
#include <tuple>
#include <deque>

#include "directed_graph.hpp"

std::tuple<std::vector<int>, std::vector<double>> generic_search_on_graph(const DirectedGraph &, const std::vector<std::vector<double>>&,
                                                                          int, const std::vector<double>&, int, unsigned int,
                                                                          double (*distance)(const std::vector<double>&, const std::vector<double>&));

std::deque<std::pair<int, double>> generic_search_on_graph_checked(const DirectedGraph &, const std::vector<std::vector<double>>&,
                                                                   int, const std::vector<double>&, int,
                                                                   double (*distance)(const std::vector<double>&, const std::vector<double>&));