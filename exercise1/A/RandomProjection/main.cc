#include <iostream>
#include <vector>

#include "hypercube.hpp"
#include "helper.hpp"

using namespace std;

int main() {
  // count time
  clock_t start = clock();

  // read MNIST data
  vector<vector<double>> mnist_data = read_mnist_data("../../MNIST/train-images-idx3-ubyte");

  // get the first 1000 mnist data
  // mnist_data = vector<vector<double>>(mnist_data.begin(), mnist_data.begin() + 1000);

  vector<vector<double>> query_data = read_mnist_data("../../MNIST/t10k-images-idx3-ubyte");

  vector<double> query = query_data[5];

  // create hypercube
  hypercube cube(mnist_data, query, 14, 1000, 1000, 10, 1000);

  // query hypercube
  vector<vector<double>> k_candidates = cube.query_n_nearest_neighbors();

  // vector<vector<double>> k_candidates = cube.query_range();

  // export results
  export_image(query, "../../output/query.ppm");
  for (int i = 0; i < (int) k_candidates.size(); i++) {
    export_image(k_candidates[i], "../../output/candidate_" + to_string(i) + ".ppm");
  }

  cout << "Done!" << endl;
  cout << "Time elapsed: " << (clock() - start) / (double) CLOCKS_PER_SEC << " seconds" << endl;
  return 0;
}