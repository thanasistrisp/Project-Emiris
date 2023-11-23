#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <limits>
#include <vector>
// cstring is used for strcmp().
// cstdlib is used for srand().
// ctime is used for time().

#include "helper.hpp"
#include "gnn.hpp"
#include "mrng.hpp"
#include "defines.hpp"
#include "handling.hpp"
#include "lsh.hpp"
#include "hypercube.hpp"

#include "brute_force.hpp"

using namespace std;
using std::cout;

vector<double> helper_arg(void *structure, const vector<vector<double>> &dataset, const vector<vector<double>> &queries, vector<int> &params)
{
	// initialize parameters
	int E = params[0];
	int R = params[1];
	int l = params[2];
	int N = params[3];
	int m = params[4];
	if (m == 1) {
		cout << "Algorithm: GNN" << endl;
	}
	else if (m == 2) {
		cout << "Algorithm: MRNG" << endl;
	}
	else if (m == 3) {
		cout << "Algorithm: LSH" << endl;
	}
	else {
		cout << "Algorithm: Cube" << endl;
	}

	double elapsed_secs_ANN = 0;
	double elapsed_secs_TNN = 0;
	double maf = 0;

	for (int q = 0; q < (int) queries.size(); q++) {
		cout << "Query: " << q << endl;
		tuple<vector<int>, vector<double>> ann;
		clock_t start_ANN = clock();
		if (m == 1) {
			ann = ((GNN*) structure)->query(queries[q], N, E, R);
		}
		else if (m == 2) {
			ann = ((MRNG*) structure)->query(queries[q], N, l);
		}
		else if (m == 3) {
			ann = ((LSH*) structure)->query(queries[q], N);
		}
		else {
			vector<int> q_proj = ((hypercube*) structure)->calculate_q_proj(queries[q]);
			ann = ((hypercube*) structure)->query(queries[q], q_proj, N);
		}
		clock_t end_ANN = clock();
		elapsed_secs_ANN += double(end_ANN - start_ANN) / CLOCKS_PER_SEC;

		clock_t start_TNN = clock();
		tuple<vector<int>, vector<double>> tnn = brute_force(dataset, queries[q], N);
		clock_t end_TNN = clock();
		elapsed_secs_TNN += double(end_TNN - start_TNN) / CLOCKS_PER_SEC;
		
        vector<int> indices_ann = get<0>(ann);
        vector<double> distances_ann = get<1>(ann);
        vector<int> indices_tnn = get<0>(tnn);
        vector<double> distances_tnn = get<1>(tnn);

		// take the minimum approximate factor over all nearest neighbors
		double temp_min = numeric_limits<double>::max();
        for(int i = 0; (unsigned int) i < indices_ann.size(); i++){
			double temp = distances_ann[i] / distances_tnn[i];
			if (temp < temp_min) {
				temp_min = temp;
			}
		}

		// maf is maximum approximate factor over all queries
		if (temp_min > maf) {
			maf = temp_min;
		}
	}
	
	return {elapsed_secs_ANN / queries.size(), elapsed_secs_TNN / queries.size(), maf};
}

extern "C" void get_gnn_results(const char *input, const char *query, int queries_num,
										  int k, int E, int R, int N, const char *load_file,
										  double *approximate_time, double *true_time, double *maf) {
	string input_str(input);
	string query_str(query);
	string load_file_str(load_file);
	
	cout << "Read MNIST data" << endl;
	vector <vector<double>> dataset = read_mnist_data(input_str);
	vector <vector<double>> queries = read_mnist_data(query_str, queries_num);
	GNN *gnn;
	if (!load_file_str.empty()) {
		cout << "Loading graph from file: " << load_file_str << endl;
		DirectedGraph *G = new DirectedGraph();
		ifstream graph_file;
		graph_file.open(load_file);
		G->load(graph_file);
		graph_file.close();
		gnn = new GNN(dataset, G);
	}
	else {
		cout << "Building graph..." << endl;
		gnn = new GNN(dataset, k);
	}
	cout << "Done" << endl;

	// return time, maf
	vector<int> params = {E, R, 0, N, 1};
	vector<double> results = helper_arg(gnn, dataset, queries, params);
	*approximate_time = results[0];
	*true_time = results[1];
	*maf = results[2];

	delete gnn;
}

extern "C" void get_mrng_results(const char *input, const char *query, int queries_num,
										  int l, int N, const char *load_file,
										  double *approximate_time, double *true_time, double *maf) {
	string input_str(input);
	string query_str(query);
	string load_file_str(load_file);
	
	cout << "Read MNIST data" << endl;
	vector <vector<double>> dataset = read_mnist_data(input_str);
	vector <vector<double>> queries = read_mnist_data(query_str, queries_num);
	MRNG *mrng;
	if (!load_file_str.empty()) {
		cout << "Loading graph from file: " << load_file_str << endl;
		DirectedGraph *G = new DirectedGraph();
		ifstream graph_file;
		graph_file.open(load_file);
		G->load(graph_file);
		graph_file.close();
		mrng = new MRNG(dataset, G);
	}
	else {
		cout << "Building graph..." << endl;
		mrng = new MRNG(dataset);
	}
	cout << "Done" << endl;

	// return time, maf
	vector<int> params = {0, 0, l, N, 2};
	vector<double> results = helper_arg(mrng, dataset, queries, params);
	*approximate_time = results[0];
	*true_time = results[1];
	*maf = results[2];

	delete mrng;
}


extern "C" void get_lsh_results(const char *input, const char *query, int queries_num,
										  int k, int L, int table_size, int window, int N,
										  double *approximate_time, double *true_time, double *maf) {
	string input_str(input);
	string query_str(query);

	cout << "Read MNIST data" << endl;
	vector <vector<double>> dataset = read_mnist_data(input_str);
	vector <vector<double>> queries = read_mnist_data(query_str, queries_num);
	LSH *lsh = new LSH(k, L, table_size, window, dataset);
	cout << "Done" << endl;

	// return time, maf
	vector<int> params = {0, 0, 0, N, 3};
	vector<double> results = helper_arg(lsh, dataset, queries, params);
	*approximate_time = results[0];
	*true_time = results[1];
	*maf = results[2];

	delete lsh;
}

extern "C" void get_hypercube_results(const char *input, const char *query, int queries_num,
										  int k, int probes, int M, int N,
										  double *approximate_time, double *true_time, double *maf) {
	string input_str(input);
	string query_str(query);

	cout << "Read MNIST data" << endl;
	vector <vector<double>> dataset = read_mnist_data(input_str);
	vector <vector<double>> queries = read_mnist_data(query_str, queries_num);
	hypercube *cube = new hypercube(dataset, k, M, probes);
	cout << "Done" << endl;

	// return time, maf
	vector<int> params = {0, 0, 0, N, 4};
	vector<double> results = helper_arg(cube, dataset, queries, params);
	*approximate_time = results[0];
	*true_time = results[1];
	*maf = results[2];

	delete cube;
}