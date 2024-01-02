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
#include "lp_metric.hpp"

#include "config.hpp"

#include "brute_force.hpp"
#include "lsh.hpp"
#include "hypercube.hpp"
#include "kmeans.hpp"
#include "approximate_knn_graph.hpp"
#include "nsg.hpp"
#include "mrng.hpp"

using namespace std;

vector<variant<double, int>> helper_arg_cluster(KMeans *structure, struct config *config, double **sil)
{
    string method_str = config->model;
    update_method method;
    tuple<int, int, int, int, int, double, int> config_tuple;
    if (method_str == "CLASSIC") {
        method = CLASSIC;
        config_tuple = make_tuple(0, 0, 0, 0, 0, 0, 0);
    }
    else if (method_str == "LSH") {
        method = REVERSE_LSH;
        config_tuple = make_tuple(config->vals[0], config->vals[1], 0, 0, 0, config->window, config->vals[2]);
    }
    else if (method_str == "CUBE") {
        method = REVERSE_HYPERCUBE;
        config_tuple = make_tuple(0, 0, config->vals[0], config->vals[1], config->vals[2], config->window, 0);
    }
    else {
        cout << "Invalid method." << endl;
        exit(1);
    }
    clock_t start = clock();
    structure->compute_clusters(10, method, config_tuple);
    clock_t end = clock();
    double clustering_time = (double)(end - start) / CLOCKS_PER_SEC;

    vector<vector<int>> clusters = structure->get_clusters();
    vector<double> si(clusters.size(), 0);
    double stotal = 0;
    for (int i = 0; i < (int) clusters.size(); i++) {
        for (int j = 0; j < (int) clusters[i].size(); j++) {
            si[i] += structure->silhouette(clusters[i][j]);
        }
        stotal += si[i];
        si[i] /= clusters[i].size();
        (*sil)[i] = si[i];
    }
    stotal /= structure->get_dataset_size();

    // Return time, aaf.
    return {clustering_time, stotal};
}

extern "C" void get_kmeans_results(struct config *config, int int_data,
								   double *clustering_time, double *stotal, double **sil)
{
    *sil = (double*) malloc(10 * sizeof(double));

	string input_str(config->dataset);
	string method_str(config->model);

	vector <vector<double>> dataset;
    if (int_data == 1) {
        dataset = read_mnist_data(input_str);
    }
    else {
        dataset = read_mnist_data_float(input_str);
    }

	KMeans *kmeans = new KMeans(dataset);

	// Return time, stotal.
	vector<variant<double, int>> results = helper_arg_cluster(kmeans, config, sil);
	*clustering_time = get<double>(results[0]);
	*stotal = get<double>(results[1]);

	delete kmeans;
}

extern "C" void free_double_array(double *sil) { free(sil); }

extern "C" void get_stotal(struct config* config, double *stotal, double *clustering_time, double **sil)
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

    vector <vector<double>> dataset = read_mnist_data_float(dataset_str);
    vector <vector<double>> decoded_dataset = read_mnist_data_float(decoded_dataset_str);

    KMeans* kmeans = new KMeans(dataset);

    string method_str = config->model;
    update_method method;
    int L = 0, k_lsh = 0, limit_queries = 0, M = 0, k_hypercube = 0, probes = 0;
    double window = 0;
    if (method_str == "CLASSIC") {
        method = CLASSIC;
    }
    else if (method_str == "LSH") {
        method = REVERSE_LSH;
        L = config->vals[0];
        k_lsh = config->vals[1];
        window = config->window;
        limit_queries = config->vals[2];
    }
    else if (method_str == "CUBE") {
        method = REVERSE_HYPERCUBE;
        M = config->vals[0];
        k_hypercube = config->vals[1];
        probes = config->vals[2];
        window = config->window;
    }
    else {
        cout << "Invalid method." << endl;
        exit(1);
    }
    tuple<int, int, int, int, int, double, int> kmean_args = make_tuple(L, k_lsh, M, k_hypercube, probes, window, limit_queries);

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