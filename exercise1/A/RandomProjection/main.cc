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

  vector<vector<double>> query_data = read_mnist_data("../../MNIST/t10k-images-idx3-ubyte");

  vector<double> query = query_data[15];

  // create hypercube
  hypercube cube(mnist_data, query, 14, 10, 2, 10, 10000);

  // query hypercube
  vector<vector<double>> k_candidates = cube.query_n_nearest_neighbors();

  // export results
  export_image(query, "../../output/query.ppm");
  for (int i = 0; i < (int) k_candidates.size(); i++) {
    export_image(k_candidates[i], "../../output/candidate_" + to_string(i) + ".png");
  }

  cout << "Done!" << endl;
  cout << "Time elapsed: " << (clock() - start) / (double) CLOCKS_PER_SEC << " seconds" << endl;
  return 0;
}