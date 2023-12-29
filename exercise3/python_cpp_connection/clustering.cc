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
using std::cout;

// #define cout if(0) cout // Comment this line to enable printing.

vector<variant<double, int>> helper_arg_cluster(void *structure, vector<variant<int, bool, string>> &params)
{
    string method_str = get<string>(params[0]);
    update_method method;
    tuple<int, int, int, int, int> config;
    if (method_str == "CLASSIC") {
        method = CLASSIC;
        config = make_tuple(0, 0, 0, 0, 0);
    }
    else if (method_str == "LSH") {
        method = REVERSE_LSH;
        config = make_tuple(get<int>(params[1]), get<int>(params[2]), 0, 0, 0);
    }
    else if (method_str == "CUBE") {
        method = REVERSE_HYPERCUBE;
        config = make_tuple(0, 0, get<int>(params[1]), get<int>(params[2]), get<int>(params[3]));
    }
    else {
        cout << "Invalid method." << endl;
        exit(1);
    }
    clock_t start = clock();
    ((KMeans*) structure)->compute_clusters(10, method, config);
    clock_t end = clock();
    double clustering_time = (double)(end - start) / CLOCKS_PER_SEC;

    vector<vector<int>> clusters = ((KMeans*) structure)->get_clusters();
    vector<double> si(clusters.size(), 0);
    double stotal = 0;
    for (int i = 0; i < (int) clusters.size(); i++) {
        for (int j = 0; j < (int) clusters[i].size(); j++) {
            si[i] += ((KMeans*) structure)->silhouette(clusters[i][j]);
        }
        stotal += si[i];
        si[i] /= clusters[i].size();
    }
    stotal /= ((KMeans*) structure)->get_dataset_size();

    // Return time, aaf.
    return {clustering_time, stotal};
}

extern "C" void get_kmeans_results(const char *input, const char *method,
                                    int L, int k_lsh, int M, int k_hypercube, int probes,
									double *clustering_time, double *stotal)
{
	string input_str(input);
	string method_str(method);

	cout << "Read MNIST data" << endl;
	vector <vector<double>> dataset = read_mnist_data_float(input_str);
	KMeans *kmeans = new KMeans(dataset);
	cout << "Done" << endl;

	// Return time, stotal.
	vector<variant<int, bool, string>> params = {method_str, L, k_lsh, M, k_hypercube, probes};
	vector<variant<double, int>> results = helper_arg_cluster(kmeans, params);
	*clustering_time = get<double>(results[0]);
	*stotal = get<double>(results[1]);

	delete kmeans;
}

extern "C" void free_double_array(double *sil) { free(sil); }

extern "C" void get_stotal(struct encoded_config* config, double *stotal, double *clustering_time, double **sil)
{
    *sil = (double*) malloc(10 * sizeof(double));

    // Initialize structure.
    string dataset_str(config->dataset);
    string decoded_dataset_str(config->decoded_dataset);

    if(!file_exists(dataset_str)){
        cout << "File " << dataset_str << " does not exist." << endl;
        exit(1);
    }
    if(!file_exists(decoded_dataset_str)){
        cout << "File " << decoded_dataset_str << " does not exist." << endl;
        exit(1);
    }

    cout << "Read MNIST data" << endl;
    vector <vector<double>> dataset = read_mnist_data_float(dataset_str);
    vector <vector<double>> decoded_dataset = read_mnist_data_float(decoded_dataset_str);

    KMeans* kmeans = new KMeans(dataset);

    string method_str = config->model;
    update_method method;
    int L = 0, k_lsh = 0, M = 0, k_hypercube = 0, probes = 0;
    if (method_str == "CLASSIC") {
        method = CLASSIC;
    }
    else if (method_str == "LSH") {
        method = REVERSE_LSH;
        L = config->enc_vals[0];
        k_lsh = config->enc_vals[1];
    }
    else if (method_str == "CUBE") {
        method = REVERSE_HYPERCUBE;
        M = config->enc_vals[0];
        k_hypercube = config->enc_vals[1];
        probes = config->enc_vals[2];
    }
    else {
        cout << "Invalid method." << endl;
        exit(1);
    }
    tuple<int, int, int, int, int> kmean_args = make_tuple(L, k_lsh, M, k_hypercube, probes);

    cout << "Running clustering algorithm..." << endl;
    clock_t start = clock();
    kmeans->compute_clusters(10, method, kmean_args);
    clock_t end = clock();
    double clustering_time_ = (double)(end - start) / CLOCKS_PER_SEC;

    vector<vector<int>> clusters = kmeans->get_clusters();
    vector<double> si(clusters.size(), 0);
    double stotal_ = 0;
    for (int i = 0; i < (int) clusters.size(); i++) {
        for (int j = 0; j < (int) clusters[i].size(); j++) {
            si[i] += kmeans->silhouette(clusters[i][j], decoded_dataset);
        }
        stotal_ += si[i];
        si[i] /= clusters[i].size();
        (*sil)[i] = si[i];
    }
    stotal_ /= kmeans->get_dataset_size();

    *stotal = stotal_;
    *clustering_time = clustering_time_;

    // Free memory.
    delete kmeans;
}