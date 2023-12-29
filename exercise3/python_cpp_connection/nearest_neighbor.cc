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
#include "handling.hpp"
#include "defines.hpp"
#include "lp_metric.hpp"

#include "encoded_config.hpp"

#include "brute_force.hpp"
#include "lsh.hpp"
#include "hypercube.hpp"
#include "kmeans.hpp"
#include "approximate_knn_graph.hpp"
#include "nsg.hpp"
#include "mrng.hpp"

using namespace std;

vector<variant<double, int>> helper_arg(void *structure, const vector<vector<double>> &dataset, const vector<vector<double>> &queries,
										vector<variant<int, bool>> &params)
{
	// Initialize parameters.
	int m = get<int>(params[0]);
	int E = get<int>(params[1]);
	int R = get<int>(params[2]);
	int l = get<int>(params[3]);
	int N = get<int>(params[4]);
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
		else if (m == 5){
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

extern "C" void get_lsh_results(const char *input, const char *query, int queries_num,
										  int k, int L, int table_size, int window, bool query_trick, int N,
										  double *approximate_time, double *aaf, int *min_neighbors) {
	string input_str(input);
	string query_str(query);

	vector <vector<double>> dataset = read_mnist_data(input_str);
	vector <vector<double>> queries = read_mnist_data(query_str, queries_num);
	LSH *lsh = new LSH(k, L, table_size, window, dataset);

	// Return time, aaf.
	vector<variant<int, bool>> params = {3, 0, 0, 0, N, query_trick};
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

	vector <vector<double>> dataset = read_mnist_data(input_str);
	vector <vector<double>> queries = read_mnist_data(query_str, queries_num);
	hypercube *cube = new hypercube(dataset, k, M, probes);

	// Return time, aaf.
	vector<variant<int, bool>> params = {4, 0, 0, 0, N};
	vector<variant<double, int>> results = helper_arg(cube, dataset, queries, params);
	*approximate_time = get<double>(results[0]);
	*aaf = get<double>(results[1]);

	delete cube;
}

extern "C" void get_gnn_results(const char *input, const char *query, int queries_num,
										  int k, int E, int R, int N, const char *load_file,
										  double *approximate_time, double *aaf) {
	string input_str(input);
	string query_str(query);
	string load_file_str(load_file);
	
	vector <vector<double>> dataset = read_mnist_data(input_str);
	vector <vector<double>> queries = read_mnist_data(query_str, queries_num);
	ApproximateKNNGraph *approximate_knn_graph;
	if (!load_file_str.empty()) {
		DirectedGraph *G = new DirectedGraph();
		ifstream graph_file;
		graph_file.open(load_file);
		G->load(graph_file);
		graph_file.close();
		approximate_knn_graph = new ApproximateKNNGraph(dataset, G);
	}
	else {
		approximate_knn_graph = new ApproximateKNNGraph(dataset, k);
	}

	// Return time, aaf.
	vector<variant<int, bool>> params = {1, E, R, 0, N};
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
	
	vector <vector<double>> dataset = read_mnist_data(input_str);
	vector <vector<double>> queries = read_mnist_data(query_str, queries_num);
	MRNG *mrng;
	if (!load_file_str.empty()) {
		DirectedGraph *G = new DirectedGraph();
		ifstream graph_file;
		graph_file.open(load_file);
		G->load(graph_file);
		graph_file.close();
		mrng = new MRNG(dataset, G);
	}
	else {
		mrng = new MRNG(dataset);
	}

	// Return time, aaf.
	vector<variant<int, bool>> params = {2, 0, 0, l, N};
	vector<variant<double, int>> results = helper_arg(mrng, dataset, queries, params);
	*approximate_time = get<double>(results[0]);
	*aaf = get<double>(results[1]);

	delete mrng;
}

extern "C" void get_nsg_results(const char *input, const char *query, int queries_num,
										  int m, int l, int lq, int k, int N, const char *load_file,
										  double *approximate_time, double *aaf) {
	string input_str(input);
	string query_str(query);
	string load_file_str(load_file);
	
	vector <vector<double>> dataset = read_mnist_data(input_str);
	vector <vector<double>> queries = read_mnist_data(query_str, queries_num);
	NSG *nsg;
	if (!load_file_str.empty()) {
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
		nsg = new NSG(dataset, l, m, k);
	}

	// Return time, aaf.
	vector<variant<int, bool>> params = {5, 0, 0, l, N, lq};
	vector<variant<double, int>> results = helper_arg(nsg, dataset, queries, params);
	*approximate_time = get<double>(results[0]);
	*aaf = get<double>(results[1]);

	delete nsg;
}

///////////////////////////////
///////////////////////////////

extern "C" void get_aaf(const char* load_file, int queries_num, struct encoded_config* config, double *aaf, double *time) {
	// Initialize structure.
	string dataset_str(config->dataset);
	string query_str(config->query);
	string encoded_dataset_str(config->encoded_dataset);
	string decoded_dataset_str(config->decoded_dataset);
	string load_file_str(load_file);

	if (!file_exists(dataset_str)) {
		cout << "File " << dataset_str << " does not exist." << endl;
		exit(1);
	}
	if (!file_exists(query_str)) {
		cout << "File " << query_str << " does not exist." << endl;
		exit(1);
	}
	if (!file_exists(encoded_dataset_str)) {
		cout << "File " << encoded_dataset_str << " does not exist." << endl;
		exit(1);
	}
	if (!file_exists(decoded_dataset_str)) {
		cout << "File " << decoded_dataset_str << " does not exist." << endl;
		exit(1);
	}
	if (!load_file_str.empty() && !file_exists(load_file_str)) {
		cout << "File " << load_file_str << " does not exist." << endl;
		exit(1);
	}

	void *structure;

	vector <vector<double>> dataset = read_mnist_data_float(dataset_str);
	vector <vector<double>> queries = read_mnist_data_float(query_str, queries_num);
	vector <vector<double>> encoded_dataset = read_mnist_data_float(encoded_dataset_str);

	if (strcmp(config->model, "LSH") == 0) {
		int k = config->enc_vals[0];
		int L = config->enc_vals[1];
		int table_size = config->enc_vals[2];
		int window = config->enc_vals[3];
		structure = new LSH(k, L, table_size, window, encoded_dataset);
		cout << "Done" << endl;
	}
	else if (strcmp(config->model, "CUBE") == 0) {
		int k = config->enc_vals[0];
		int M = config->enc_vals[1];
		int probes = config->enc_vals[2];
		structure = new hypercube(encoded_dataset, k, M, probes);
		cout << "Done" << endl;
	}
	else if (strcmp(config->model, "GNNS") == 0) {
		int k = config->enc_vals[0];
		ApproximateKNNGraph *approximate_knn_graph;
		if (!load_file_str.empty()) {
			DirectedGraph *G = new DirectedGraph();
			ifstream graph_file;
			graph_file.open(load_file);
			G->load(graph_file);
			graph_file.close();
			approximate_knn_graph = new ApproximateKNNGraph(encoded_dataset, G);
		}
		else {
			approximate_knn_graph = new ApproximateKNNGraph(encoded_dataset, k);
		}
		structure = approximate_knn_graph;
	}
	else if (strcmp(config->model, "MRNG") == 0) {
		MRNG *mrng;
		if(!load_file_str.empty()){
			DirectedGraph *G = new DirectedGraph();
			ifstream graph_file;
			graph_file.open(load_file);
			G->load(graph_file);
			graph_file.close();
			mrng = new MRNG(dataset, G);
		}
		else{
			mrng = new MRNG(encoded_dataset);
		}
		structure = mrng;
	}
	else if (strcmp(config->model, "NSG") == 0) {
		int l = config->enc_vals[0];
		int m = config->enc_vals[1];
		int k = config->enc_vals[2];
		NSG *nsg;
		if(!load_file_str.empty()){
			DirectedGraph *G = new DirectedGraph();
			ifstream graph_file;
			graph_file.open(load_file);
			G->load(graph_file);
			int navigating_node;
			graph_file.read((char*) &navigating_node, sizeof(int));
			graph_file.close();
			nsg = new NSG(encoded_dataset, G, navigating_node);
		}
		else{
			nsg = new NSG(encoded_dataset, l, m, k);
		}
		structure = nsg;
	}
	else if (strcmp(config->model, "BRUTE") == 0) {
		structure = NULL;
	}
	else {
		cout << "Unknown model" << endl;
		exit(1);
	}

	double aaf_ = 0;
	double time_ = 0;
	if (queries_num == -1)
		queries_num = queries.size();
	for (int q = 0; q < queries_num; q++) {
		vector<double> query_init = queries[q];
		tuple<vector<int>, vector<double>> true_nn_init_ = brute_force(dataset, query_init, 1);
		vector<double> true_nn_init = dataset[get<0>(true_nn_init_)[0]];
		vector<double> query_enc = encoded_dataset[q];
		tuple<vector<int>, vector<double>> ann_enc_;

		clock_t start_ANN = clock();

		if (strcmp(config->model, "LSH") == 0) {
			bool query_trick = config->enc_vals[4];
			ann_enc_ = ((LSH*) structure)->query(query_enc, 1, euclidean_distance, query_trick);
		}
		else if (strcmp(config->model, "CUBE") == 0) {
			vector<int> q_proj = ((hypercube*) structure)->calculate_q_proj(query_enc);
			ann_enc_ = ((hypercube*) structure)->query(query_enc, q_proj, 1);
		}
		else if (strcmp(config->model, "GNNS") == 0) {
			int N = config->enc_vals[0];
			int E = config->enc_vals[1];
			int R = config->enc_vals[3];
			ann_enc_ = ((ApproximateKNNGraph*) structure)->query(query_enc, N, E, R);
		}
		else if (strcmp(config->model, "MRNG") == 0) {
			int l = config->enc_vals[0];
			ann_enc_ = ((MRNG*) structure)->query(query_enc, 1, l);
		}
		else if (strcmp(config->model, "NSG") == 0) {
			int lq = config->enc_vals[4];
			ann_enc_ = ((NSG*) structure)->query(query_enc, 1, lq);
		}
		else if (strcmp(config->model, "BRUTE") == 0) {
			ann_enc_ = brute_force(encoded_dataset, query_enc, 1);
		}
		else {
			cout << "Unknown model" << endl;
			exit(1);
		}

		clock_t end_ANN = clock();

		time_ += double(end_ANN - start_ANN) / CLOCKS_PER_SEC;

		vector<double> ann_enc = encoded_dataset[get<0>(ann_enc_)[0]];
		vector<double> ann_init = get_mnist_float_index(decoded_dataset_str, get<0>(ann_enc_)[0]);

		aaf_ += euclidean_distance(query_init, ann_init) / euclidean_distance(query_init, true_nn_init);
	}
	*aaf = aaf_ / queries_num;
	*time = time_ / queries_num;

	// Free memory.
	if (strcmp(config->model, "CUBE") == 0) {
		delete (hypercube*) structure;
	}
	else if (strcmp(config->model, "LSH") == 0) {
		delete (LSH*) structure;
	}
	else if (strcmp(config->model, "MRNG") == 0) {
		delete (MRNG*) structure;
	}
	else if (strcmp(config->model, "NSG") == 0) {
		delete (NSG*) structure;
	}
	else if (strcmp(config->model, "GNNS") == 0) {
		delete (ApproximateKNNGraph*) structure;
	}
}