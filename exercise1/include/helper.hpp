#pragma once

#include <vector>
#include <string>
#include <tuple>

std::vector<std::vector<double>> read_mnist_data(const std::string &filename, int num=0); // does not check if file exists
std::tuple<int, int, int, int, int, int> read_config_file(const std::string &filename); // does not check if file exists
void export_image(const std::vector<double> &image, std::string filename);
bool file_exists(const std::string &filename);