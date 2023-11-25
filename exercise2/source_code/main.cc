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
using std::cout;

int main(int argc, char *argv[]) {
	srand(time(NULL));

	string input_file;
	string query_file;
	string output_file;
	string save_graph_file;
	string load_graph_file;
	int k = 50;
	int E = 30;
	int R = 1;
	int l = 20;
	int N = 1;
	int m = 0;

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
		else if (strcmp(argv[i], "-R") == 0) {
			R = atoi(argv[i + 1]);
			i++;
		}
		else if (strcmp(argv[i], "-l") == 0) {
			l = atoi(argv[i + 1]);
			i++;
		}
		else if (strcmp(argv[i], "-N") == 0) {
			N = atoi(argv[i + 1]);
			i++;
		}
		else if (strcmp(argv[i], "-m") == 0) {
			m = atoi(argv[i + 1]);
			i++;
		}
		else if (strcmp(argv[i], "-o") == 0) {
			output_file = argv[i + 1];
			i++;
		}
		else if (strcmp(argv[i], "-save") == 0) {
			save_graph_file = argv[i + 1];
			i++;
		}
		else if (strcmp(argv[i], "-load") == 0) {
			load_graph_file = argv[i + 1];
			i++;
		}
		else if (strcmp(argv[i], "-help") == 0) {
			cout << "Usage: ./graph_search -d <input file> -q <query file> -k <int> -E <int> -R <int> -N <int> -l <int, only for Search-on-Graph> "\
				    "-m <1 for GNNS, 2 for MRNG> -o <output file> -save <save graph file> -load <load graph file>" << endl;
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

	if (!file_exists(input_file) || !file_exists(query_file)) {
		cout << "File cannot be found" << endl;
		return 1;
	}

	if (!load_graph_file.empty() && !file_exists(load_graph_file)) {
		cout << "File cannot be found" << endl;
		return 1;
	}

	cout << "Read MNIST data" << endl;
	vector <vector<double>> dataset = read_mnist_data(input_file);

	cout << "Creating structure" << endl;

	time_t start1, end1;
	time(&start1);

	void *structure;
	vector<int> params = {E, R, l, N, m};

	// load file if string not empty
	if (!load_graph_file.empty()) {
		ifstream graph_file(load_graph_file, ios::binary);
		DirectedGraph *G = new DirectedGraph();
		G->load(graph_file);
		graph_file.close();
		// add graph to structure
		switch (m) {
			case 1:
				structure = new GNN(dataset, G);
				break;
			case 2:
				structure = new MRNG(dataset, G);
				break;
			default:
				cout << "Wrong m value. Run with -help for more info" << endl;
				exit(1);
		}
	}
	else {
		switch (m) {
			case 1:
				structure = new GNN(dataset, k);
				break;
			case 2:
				structure = new MRNG(dataset);
				break;
			default:
				cout << "Wrong m value. Run with -help for more info" << endl;
				exit(1);
		}
	}

	time(&end1);

	// save Graph to binary file
	if (!save_graph_file.empty()) {
		ofstream graph_file(save_graph_file, ios::binary);
		((DirectedGraph*) ((m == 1) ? ((GNN*) structure)->get_graph() : ((MRNG*) structure)->get_graph()))->save(graph_file);
		graph_file.close();
	}


	cout << "Structure created in " << difftime(end1, start1) << " seconds" << endl;

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
		queries = read_mnist_data(query_file, 10);

		handle_ouput(structure, dataset, queries, output, params);

		end = clock();
		elapsed_secs += double(end - start) / CLOCKS_PER_SEC;

		cont:
			cout << "Enter query file: ";
			cin >> query_file;
	}

	switch (m) {
		case 1:
			delete (GNN*) structure;
			break;
		case 2:
			delete (MRNG*) structure;
			break;
	}

	cout << "Done in " << elapsed_secs << " seconds" << endl;

	return 0;
}