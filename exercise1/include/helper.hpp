#pragma once

#include <vector>
#include <string>

std::vector<std::vector<double>> read_mnist_data(std::string filename); // does not check if file exists
void export_image(std::vector<double> image, std::string filename);
void handle_ouput(hypercube &cube, std::ofstream &output, const std::vector<std::vector<double>> &queries);
bool file_exists(std::string filename);