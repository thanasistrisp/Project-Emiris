#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <vector>
// cstring is used for strcmp().
// cstdlib is used for srand().
// ctime is used for time().

#include "helper.hpp"
#include "gnn.hpp"
#include "mrng.hpp"
#include "defines.hpp"
#include "handling.hpp"

using namespace std;

int main(int argc, char *argv[]) {
	srand(time(NULL));

	string input_file;
	string query_file;
	string output_file;
	int k = 50;
	int E = 30;
	int R = 1;
	int N = 1;

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
		else if (strcmp(argv[i], "-E") == 0) {
			E = atoi(argv[i + 1]);
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
			cout << "Usage: ./lsh -d <input file> -q <query file> -k <int> -M <int> -probes <int> -o <output file> -N <int> -R <double>" << endl;
			return 0;
		}
		else {
			cout << "Invalid arguments" << endl;
			return 1;
		}
	}

	// ask from user
	if (input_file.empty()) {
		cout << "Enter input file: ";
		cin >> input_file;
	}

	if (query_file.empty()) {
		cout << "Enter query file: ";
		cin >> query_file;
	}

	if (output_file.empty()) {
		cout << "Enter output file: ";
		cin >> output_file;
	}

	if (!file_exists(input_file)) {
		cout << "File " << input_file << " does not exist" << endl;
		exit(1);
	}
	vector <vector<double>> dataset = read_mnist_data(input_file, 100);

	cout << "Read MNIST data" << endl;

	cout << "Creating GNN" << endl;

	time_t start1, end1;
	time(&start1);

	MRNG mrng(k, dataset, R, E);

	time(&end1);


	cout << "GNN created in " << difftime(end1, start1) << " seconds" << endl;

	ofstream output(output_file);

	double elapsed_secs = 0;

	vector <vector<double>> queries;

	clock_t start, end;

	while (query_file != "exit" && !cin.eof()) {

		start = clock();

		if (!file_exists(query_file)) {
			cout << "File " << query_file << " does not exist" << endl;
			end = clock();
			goto cont;
		}
		queries = read_mnist_data(query_file);
		queries.resize(10);

		handle_ouput(mrng, dataset, queries, N, output);

		end = clock();
		elapsed_secs += double(end - start) / CLOCKS_PER_SEC;

		cont:
			cout << "Enter query file: ";
			cin >> query_file;
	}

	cout << "Done in " << elapsed_secs << " seconds" << endl;

	return 0;
}