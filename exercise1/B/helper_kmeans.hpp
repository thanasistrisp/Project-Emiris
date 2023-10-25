#pragma once

#include <vector>
#include <string>
#include <tuple>

#include "kmeans.hpp"

void handle_cluster_output(KMeans &kmeans, const std::string &output_file, bool complete, update_method method, const std::tuple<int,int,int,int,int,int> &config);