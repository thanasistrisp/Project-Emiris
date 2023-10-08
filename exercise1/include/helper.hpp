#pragma once

#include <vector>
#include <string>

std::vector<std::vector<double>> read_mnist_data(std::string filename);
void export_image(std::vector<double> image, std::string filename);