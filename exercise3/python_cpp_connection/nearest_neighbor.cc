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

#define cout if(0) cout // Comment this line to enable printing.

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
	double aaf = 0; // Average approximate factor.
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

		// Take the average approximate factor from all neighbors.
		double af = 0;
		for (int i = 0; i < (int) indices_ann.size(); i++) {
			af += distances_ann[i] / distances_tnn[i];
		}
		af /= indices_ann.size();
		aaf += af;
	}
	
	return {elapsed_secs_ANN / queries.size(), aaf / queries.size(), min_neighbors};
}

extern "C" void get_gnn_results(const char *input, const char *query, int queries_num,
										  int k, int E, int R, int N, const char *load_file,
										  double *approximate_time, double *aaf) {
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

	// Return time, aaf.
	vector<variant<int,bool>> params = {E, R, 0, N, 1};
	vector<variant<double, int>> results = helper_arg(approximate_knn_graph, dataset, queries, params);
	*approximate_time = get<double>(results[0]);
	*aaf = get<double>(results[1]);

	delete approximate_knn_graph;
}

extern "C" void get_mrng_results(const char *input, const char *query, int queries_num,
										  int l, int N, const char *load_file,
										  double *approximate_time, double *aaf) {
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

	// Return time, aaf.
	vector<variant<int,bool>> params = {0, 0, l, N, 2};
	vector<variant<double, int>> results = helper_arg(mrng, dataset, queries, params);
	*approximate_time = get<double>(results[0]);
	*aaf = get<double>(results[1]);

	delete mrng;
}


extern "C" void get_lsh_results(const char *input, const char *query, int queries_num,
										  int k, int L, int table_size, int window, bool query_trick, int N,
										  double *approximate_time, double *aaf, int *min_neighbors) {
	string input_str(input);
	string query_str(query);

	cout << "Read MNIST data" << endl;
	vector <vector<double>> dataset = read_mnist_data(input_str);
	vector <vector<double>> queries = read_mnist_data(query_str, queries_num);
	LSH *lsh = new LSH(k, L, table_size, window, dataset);
	cout << "Done" << endl;

	// Return time, aaf.
	vector<variant<int,bool>> params = {0, 0, 0, N, 3, query_trick};
	vector<variant<double, int>> results = helper_arg(lsh, dataset, queries, params);
	*approximate_time = get<double>(results[0]);
	*aaf = get<double>(results[1]);
	*min_neighbors = get<int>(results[2]);

	delete lsh;
}

extern "C" void get_hypercube_results(const char *input, const char *query, int queries_num,
										  int k, int probes, int M, int N,
										  double *approximate_time, double *aaf) {
	string input_str(input);
	string query_str(query);

	cout << "Read MNIST data" << endl;
	vector <vector<double>> dataset = read_mnist_data(input_str);
	vector <vector<double>> queries = read_mnist_data(query_str, queries_num);
	hypercube *cube = new hypercube(dataset, k, M, probes);
	cout << "Done" << endl;

	// Return time, aaf.
	vector<variant<int,bool>> params = {0, 0, 0, N, 4};
	vector<variant<double, int>> results = helper_arg(cube, dataset, queries, params);
	*approximate_time = get<double>(results[0]);
	*aaf = get<double>(results[1]);

	delete cube;
}

extern "C" void get_nsg_results(const char *input, const char *query, int queries_num,
										  int m, int l, int lq, int k, int N, const char *load_file,
										  double *approximate_time, double *aaf) {
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

	// Return time, aaf.
	vector<variant<int,bool>> params = {0, 0, l, N, 5, lq};
	vector<variant<double, int>> results = helper_arg(nsg, dataset, queries, params);
	*approximate_time = get<double>(results[0]);
	*aaf = get<double>(results[1]);

	delete nsg;
}

///////////////////////////////
///////////////////////////////

struct CA {
   char* model;
   int *vals;
};

extern "C" void get_nearest_neighbor(const char* input, const char* query_string, int query_index, const char* load_file, struct CA* ca, double *approximate_time, int *index) {
	string input_str(input);
	string query_str(query_string);
	string load_file_str(load_file);

	cout << "Read MNIST data" << endl;
	vector <vector<double>> dataset = read_mnist_data(input_str);
	vector<double> query = get_mnist_index(query_str, query_index);

	if (strcmp(ca->model, "CUBE") == 0) { // k, M, probes
		hypercube *cube = new hypercube(dataset, ca->vals[0], ca->vals[1], ca->vals[2]);
		cout << "Done" << endl;

		vector<int> q_proj = cube->calculate_q_proj(query);

		clock_t start_ANN = clock();
		tuple<vector<int>, vector<double>> ann = cube->query(query, q_proj, 1);
		clock_t end_ANN = clock();

		*index = get<0>(ann)[0];
		*approximate_time = double(end_ANN - start_ANN) / CLOCKS_PER_SEC;
	}
	else if (strcmp(ca->model, "LSH") == 0) { // k, L, table_size, window, query_trick
		LSH *lsh = new LSH(ca->vals[0], ca->vals[1], ca->vals[2], ca->vals[3], dataset);
		cout << "Done" << endl;

		clock_t start_ANN = clock();
		tuple<vector<int>, vector<double>> ann = lsh->query(query, 1, euclidean_distance, ca->vals[4]);
		clock_t end_ANN = clock();

		*index = get<0>(ann)[0];
		*approximate_time = double(end_ANN - start_ANN) / CLOCKS_PER_SEC;
	}
	else if (strcmp(ca->model, "MRNG") == 0) { // l
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

		clock_t start_ANN = clock();
		tuple<vector<int>, vector<double>> ann = mrng->query(query, 1, ca->vals[0]);
		clock_t end_ANN = clock();

		*index = get<0>(ann)[0];
		*approximate_time = double(end_ANN - start_ANN) / CLOCKS_PER_SEC;
	}
	else if (strcmp(ca->model, "NSG") == 0) { // l, m, k, lq
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
			nsg = new NSG(dataset, ca->vals[0], ca->vals[1], ca->vals[2]);
		}
		cout << "Done" << endl;

		clock_t start_ANN = clock();
		tuple<vector<int>, vector<double>> ann = nsg->query(query, 1, ca->vals[3]);
		clock_t end_ANN = clock();

		*index = get<0>(ann)[0];
		*approximate_time = double(end_ANN - start_ANN) / CLOCKS_PER_SEC;
	}
	else if (strcmp(ca->model, "GNN") == 0) { // k, E, R
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
			approximate_knn_graph = new ApproximateKNNGraph(dataset, ca->vals[0]);
		}
		cout << "Done" << endl;

		clock_t start_ANN = clock();
		tuple<vector<int>, vector<double>> ann = approximate_knn_graph->query(query, 1, ca->vals[1], ca->vals[2]);
		clock_t end_ANN = clock();

		*index = get<0>(ann)[0];
		*approximate_time = double(end_ANN - start_ANN) / CLOCKS_PER_SEC;
	}
	else if (strcmp(ca->model, "BRUTE") == 0) {
		clock_t start_ANN = clock();
		tuple<vector<int>, vector<double>> ann = brute_force(dataset, query, 1);
		clock_t end_ANN = clock();

		*index = get<0>(ann)[0];
		*approximate_time = double(end_ANN - start_ANN) / CLOCKS_PER_SEC;
	
	}
	else {
		cout << "Unknown model" << endl;
		exit(1);
	}
}