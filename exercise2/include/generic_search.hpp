#pragma once

#include <vector>
#include <tuple>

#include "directed_graph.hpp"

std::tuple<std::vector<int>, std::vector<double>> generic_search_on_graph(DirectedGraph &, int, std::vector<double>&, int, int);