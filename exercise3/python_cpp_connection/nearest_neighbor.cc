#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <variant>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <limits>
#include <vector>

#include "helper.hpp"
#include "approximate_knn_graph.hpp"
#include "nsg.hpp"
#include "mrng.hpp"
#include "defines.hpp"
#include "handling.hpp"
#include "lsh.hpp"
#include "hypercube.hpp"
#include "lp_metric.hpp"

#include "brute_force.hpp"

using namespace std;
using std::cout;

vector<variant<double, int>> helper_arg(void *structure, const vector<vector<double>> &dataset, const vector<vector<double>> &queries, vector<variant<int,bool>> &params)
{
	// Initialize parameters.
	int E = get<int>(params[0]);
	int R = get<int>(params[1]);
	int l = get<int>(params[2]);
	int N = get<int>(params[3]);
	int m = get<int>(params[4]);
	if (m == 1) {
		cout << "Algorithm: GNN" << endl;
	}
	else if (m == 2) {
		cout << "Algorithm: MRNG" << endl;
	}
	else if (m == 3) {
		cout << "Algorithm: LSH" << endl;
	}
	else if (m == 4) {
		cout << "Algorithm: Cube" << endl;
	}
	else if (m == 5) {
		cout << "Algorithm: NSG" << endl;
	}
	else {
		cout << "Algorithm: Unknown" << endl;
		exit(1);
	}

	double elapsed_secs_ANN = 0;
	double maf = 1;
	int min_neighbors = numeric_limits<int>::max();

	for (int q = 0; q < (int) queries.size(); q++) {
		cout << "Query: " << q << endl;
		tuple<vector<int>, vector<double>> ann;
		clock_t start_ANN = clock();
		if (m == 1) {
			ann = ((ApproximateKNNGraph*) structure)->query(queries[q], N, E, R);
		}
		else if (m == 2) {
			ann = ((MRNG*) structure)->query(queries[q], N, l);
		}
		else if (m == 3) {
			bool query_trick = get<bool>(params[5]);
			ann = ((LSH*) structure)->query(queries[q], N, euclidean_distance, query_trick);
		}
		else if (m == 4) {
			vector<int> q_proj = ((hypercube*) structure)->calculate_q_proj(queries[q]);
			ann = ((hypercube*) structure)->query(queries[q], q_proj, N);
		}
		else {
			int lq = get<int>(params[5]);
			ann = ((NSG*) structure)->query(queries[q], N, lq);
		}

		if (q == 0) {
			int temp = get<0>(ann).size();
			min_neighbors = temp;
		}
		else {
			int temp = get<0>(ann).size();
			if (temp < min_neighbors) {
				min_neighbors = temp;
			}
		}


		clock_t end_ANN = clock();
		elapsed_secs_ANN += double(end_ANN - start_ANN) / CLOCKS_PER_SEC;

		tuple<vector<int>, vector<double>> tnn = brute_force(dataset, queries[q], N);
		
        vector<int> indices_ann = get<0>(ann);
        vector<double> distances_ann = get<1>(ann);
        vector<int> indices_tnn = get<0>(tnn);
        vector<double> distances_tnn = get<1>(tnn);

		// Take the minimum approximate factor from all neighbors.
		int distance_min = distances_ann[0];

		// MAF is maximum approximate factor over all queries.
		double approximate_factor = (double) distance_min / distances_tnn[0];
		if (approximate_factor > maf) {
			maf = approximate_factor;
		}
	}
	
	return {elapsed_secs_ANN / queries.size(), maf, min_neighbors};
}

extern "C" void get_gnn_results(const char *input, const char *query, int queries_num,
										  int k, int E, int R, int N, const char *load_file,
										  double *approximate_time, double *maf) {
	string input_str(input);
	string query_str(query);
	string load_file_str(load_file);
	
	cout << "Read MNIST data" << endl;
	vector <vector<double>> dataset = read_mnist_data(input_str);
	vector <vector<double>> queries = read_mnist_data(query_str, queries_num);
	ApproximateKNNGraph *approximate_knn_graph;
	if (!load_file_str.empty()) {
		cout << "Loading graph from file: " << load_file_str << endl;
		DirectedGraph *G = new DirectedGraph();
		ifstream graph_file;
		graph_file.open(load_file);
		G->load(graph_file);
		graph_file.close();
		approximate_knn_graph = new ApproximateKNNGraph(dataset, G);
	}
	else {
		cout << "Building graph..." << endl;
		approximate_knn_graph = new ApproximateKNNGraph(dataset, k);
	}
	cout << "Done" << endl;

	// Return time, MAF.
	vector<variant<int,bool>> params = {E, R, 0, N, 1};
	vector<variant<double, int>> results = helper_arg(approximate_knn_graph, dataset, queries, params);
	*approximate_time = get<double>(results[0]);
	*maf = get<double>(results[1]);

	delete approximate_knn_graph;
}

extern "C" void get_mrng_results(const char *input, const char *query, int queries_num,
										  int l, int N, const char *load_file,
										  double *approximate_time, double *maf) {
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

	// Return time, MAF.
	vector<variant<int,bool>> params = {0, 0, l, N, 2};
	vector<variant<double, int>> results = helper_arg(mrng, dataset, queries, params);
	*approximate_time = get<double>(results[0]);
	*maf = get<double>(results[1]);

	delete mrng;
}


extern "C" void get_lsh_results(const char *input, const char *query, int queries_num,
										  int k, int L, int table_size, int window, bool query_trick, int N,
										  double *approximate_time, double *maf, int *min_neighbors) {
	string input_str(input);
	string query_str(query);

	cout << "Read MNIST data" << endl;
	vector <vector<double>> dataset = read_mnist_data(input_str);
	vector <vector<double>> queries = read_mnist_data(query_str, queries_num);
	LSH *lsh = new LSH(k, L, table_size, window, dataset);
	cout << "Done" << endl;

	// Return time, MAF.
	vector<variant<int,bool>> params = {0, 0, 0, N, 3, query_trick};
	vector<variant<double, int>> results = helper_arg(lsh, dataset, queries, params);
	*approximate_time = get<double>(results[0]);
	*maf = get<double>(results[1]);
	*min_neighbors = get<int>(results[2]);

	delete lsh;
}

extern "C" void get_hypercube_results(const char *input, const char *query, int queries_num,
										  int k, int probes, int M, int N,
										  double *approximate_time, double *maf) {
	string input_str(input);
	string query_str(query);

	cout << "Read MNIST data" << endl;
	vector <vector<double>> dataset = read_mnist_data(input_str);
	vector <vector<double>> queries = read_mnist_data(query_str, queries_num);
	hypercube *cube = new hypercube(dataset, k, M, probes);
	cout << "Done" << endl;

	// Return time, MAF.
	vector<variant<int,bool>> params = {0, 0, 0, N, 4};
	vector<variant<double, int>> results = helper_arg(cube, dataset, queries, params);
	*approximate_time = get<double>(results[0]);
	*maf = get<double>(results[1]);

	delete cube;
}

extern "C" void get_nsg_results(const char *input, const char *query, int queries_num,
										  int m, int l, int lq, int k, int N, const char *load_file,
										  double *approximate_time, double *maf) {
	string input_str(input);
	string query_str(query);
	string load_file_str(load_file);
	
	cout << "Read MNIST data" << endl;
	vector <vector<double>> dataset = read_mnist_data(input_str);
	vector <vector<double>> queries = read_mnist_data(query_str, queries_num);
	NSG *nsg;
	if (!load_file_str.empty()) {
		cout << "Loading graph from file: " << load_file_str << endl;
		DirectedGraph *G = new DirectedGraph();
		ifstream graph_file;
		graph_file.open(load_file);
		G->load(graph_file);
		int navigating_node;
		graph_file.read((char*) &navigating_node, sizeof(int));
		graph_file.close();
		nsg = new NSG(dataset, G, navigating_node);
	}
	else {
		cout << "Building graph..." << endl;
		nsg = new NSG(dataset, l, m, k);
	}
	cout << "Done" << endl;

	// Return time, MAF.
	vector<variant<int,bool>> params = {0, 0, l, N, 5, lq};
	vector<variant<double, int>> results = helper_arg(nsg, dataset, queries, params);
	*approximate_time = get<double>(results[0]);
	*maf = get<double>(results[1]);

	delete nsg;
}