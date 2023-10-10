#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <fstream>

#include "hypercube.hpp"
#include "helper.hpp"

using namespace std;

int main(int argc, char *argv[]) {
	string input_file;
	string query_file;
	string output_file;
	int k = 14;
	int M = 10;
	int probes = 2;
	int N = 1;
	double R = 10000;

	for (int i = 1; i < argc; i++) {
		if (strcmp(argv[i], "-d") == 0) {
			input_file = argv[i + 1];
			i++;
		}
		else if (strcmp(argv[i], "-q") == 0) {
			query_file = argv[i + 1];
			i++;
		}
		else if (strcmp(argv[i], "-k") == 0) {
			k = atoi(argv[i + 1]);
			i++;
		}
		else if (strcmp(argv[i], "-M") == 0) {
			M = atoi(argv[i + 1]);
			i++;
		}
		else if (strcmp(argv[i], "-probes") == 0) {
			probes = atoi(argv[i + 1]);
			i++;
		}
		else if (strcmp(argv[i], "-N") == 0) {
			N = atoi(argv[i + 1]);
			i++;
		}
		else if (strcmp(argv[i], "-R") == 0) {
			R = atof(argv[i + 1]);
			i++;
		}
		else if (strcmp(argv[i], "-o") == 0) {
			output_file = argv[i + 1];
			i++;
		}
		else if (strcmp(argv[i], "-help") == 0) {
			cout << "Usage: ./lsh -d <input file> -q <query file> [-k <int>] [-M <int>] [-probes <int>] -o <output file> [-N <int>] [-R <double>]" << endl;
			return 0;
		}
		else {
			cout << "Invalid arguments" << endl;
			return 1;
		}
	}

	if (input_file.empty() || query_file.empty() || output_file.empty()) {
		cout << "Mandatory arguments missing" << endl;
		return 1;
	}

	// print all
	cout << "input_file: " << input_file << endl;
	cout << "query_file: " << query_file << endl;
	cout << "output_file: " << output_file << endl;
	cout << "k: " << k << endl;
	cout << "M: " << M << endl;
	cout << "probes: " << probes << endl;
	cout << "N: " << N << endl;
	cout << "R: " << R << endl;
}