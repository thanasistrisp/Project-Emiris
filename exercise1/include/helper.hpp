#pragma once

#include <vector>
#include <string>

std::vector<std::vector<double>> read_mnist_data(const std::string &filename); // does not check if file exists
void export_image(const std::vector<double> &image, std::string filename);
void handle_ouput(hypercube &cube, std::ofstream &output, const std::vector<std::vector<double>> &queries);
bool file_exists(const std::string &filename);