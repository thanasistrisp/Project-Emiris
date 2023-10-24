#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <algorithm>
#include <tuple>
#include <random>

#include "kmeans.h"
#include "helper_kmeans.hpp"
#include "helper.hpp"

using namespace std;

int main(int argc, char *argv[]) {
	srand(time(NULL));

	string input_file;
	string config_file;
	string output_file;
	bool complete = false;
	string method_str = "";

	for (int i = 1; i < argc; i++) {
		if (strcmp(argv[i], "-i") == 0) {
			input_file = argv[i + 1];
			i++;
		}
		else if (strcmp(argv[i], "-c") == 0) {
			config_file = argv[i + 1];
			i++;
		}
		else if (strcmp(argv[i], "-o") == 0) {
			output_file = argv[i + 1];
			i++;
		}
		else if (strcmp(argv[i], "-complete") == 0) {
			complete = true;
		}
		else if (strcmp(argv[i], "-m") == 0) {
			method_str = argv[i + 1];
			i++;
		}
		else if (strcmp(argv[i], "-help") == 0) {
			cout << "Usage: ./cluster -i <input file> -c <configuration file> -o <output file> -complete <optional> -m <method: Classic OR LSH or Hypercube>" << endl;
			return 0;
		}
		else {
			cout << "Invalid arguments" << endl;
			return 1;
		}
	}

	
	// check if files exist
	if (!file_exists(input_file)) {
		cout << "Input file does not exist" << endl;
		return 1;
	}
	if (!file_exists(config_file)) {
		cout << "Query file does not exist" << endl;
		return 1;
	}

	// convert method string to enum
	transform(method_str.begin(), method_str.end(), method_str.begin(), ::tolower); // convert to lowercase
	update_method method;
	if (method_str == "classic") {
		method = CLASSIC;
	}
	else if (method_str == "lsh") {
		method = REVERSE_LSH;
	}
	else if (method_str == "hypercube") {
		method = REVERSE_HYPERCUBE;
	}
	else {
		cout << "Invalid method" << endl;
		return 1;
	}

	// read input file
	vector<vector<double>> dataset = read_mnist_data(input_file);
	
	// read config file
	tuple<int, int, int, int, int, int> config = read_config_file(config_file);

	// run kmeans
	KMeans kmeans(dataset);

	handle_cluster_output(kmeans, output_file, complete, method, config);
}