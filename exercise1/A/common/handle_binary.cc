#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

static int reverse_int(int);

vector<vector<double>> read_mnist_data(string filename) {
  // read MNIST data from file
  ifstream file(filename, ios::binary);

  // read magic number
  int magic_number = 0;
  file.read((char*)&magic_number, sizeof(int));
  magic_number = reverse_int(magic_number); // ignore it for our purposes

  // read number of images
  int number_of_images = 0;
  file.read((char*)&number_of_images, sizeof(int));
  number_of_images = reverse_int(number_of_images);

  // read number of rows
  int rows = 0;
  file.read((char*)&rows, sizeof(int));
  rows = reverse_int(rows);

  // read number of columns
  int cols = 0;
  file.read((char*)&cols, sizeof(int));
  cols = reverse_int(cols);

  // read data
  vector<vector<double>> mnist_data(number_of_images, vector<double>(rows * cols));
  for (int i = 0; i < number_of_images; i++) {
    for (int r = 0; r < rows; r++) {
      for (int c = 0; c < cols; c++) {
        unsigned char temp = 0;
        file.read((char*)&temp, sizeof(unsigned char));
        mnist_data[i][(rows * r) + c] = (double)temp;
      }
    }
  }

  return mnist_data;
}

static int reverse_int(int i) {
  unsigned char c1, c2, c3, c4;

  c1 = i & 255;
  c2 = (i >> 8) & 255;
  c3 = (i >> 16) & 255;
  c4 = (i >> 24) & 255;

  return ((int)c1 << 24) + ((int)c2 << 16) + ((int)c3 << 8) + c4;
}

void export_image(vector<double> image, string filename) {
  // export image to file
  ofstream file(filename, ios::binary);

  // write header
  file << "P6" << endl;
  file << 28 << " " << 28 << endl;
  file << 255 << endl;

  // write data
  for (int i = 0; i < (int) image.size(); i++) {
    unsigned char temp = (unsigned char) image[i];
    file.write((char*)&temp, sizeof(unsigned char));
    file.write((char*)&temp, sizeof(unsigned char));
    file.write((char*)&temp, sizeof(unsigned char));
  }
}