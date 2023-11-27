#pragma once

#include <unordered_set>

#include "directed_graph.hpp"

std::unordered_set<int> depth_first_search(const DirectedGraph &, int);