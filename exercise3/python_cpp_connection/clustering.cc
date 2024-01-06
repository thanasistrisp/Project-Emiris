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
#include <cmath>

#include "helper.hpp"
#include "handling.hpp"
#include "lp_metric.hpp"

#include "config.hpp"

#include "brute_force.hpp"
#include "lsh.hpp"
#include "hypercube.hpp"
#include "kmeans.hpp"
#include "kmeans_eval.hpp"
#include "approximate_knn_graph.hpp"
#include "nsg.hpp"
#include "mrng.hpp"

using namespace std;

// Connector functions that execute the K-Means algorithm in latent space and return
// the clustering time, total and per-cluster silhouette in latent space and the silhouette between the two spaces.

double compute_objective_function(const vector<vector<double>>& dataset, const std::vector<std::vector<int>> clusters,
                                  const vector<vector<double>>& centroids)
{
    // Compute J = \sum_{j=1}^{k} \sum_{x_i \in c_j} || x_i - c_j ||^{2}.
    double obj = 0.0;
    for(int j = 0; j < centroids.size(); j++){
        for(int i = 0; i < clusters[j].size(); i++){
            double dist = euclidean_distance(dataset[clusters[j][i]], centroids[j]);
            obj += (dist * dist);
        }
    }
    return obj;
}

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

    // Return time, stotal.
    return {clustering_time, stotal};
}

extern "C" void get_kmeans_results(struct config *config, int int_data,
								   double *clustering_time, double *stotal, double **sil, double *obj_func)
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

    // Compute objective function.
    *obj_func = compute_objective_function(dataset, kmeans->get_clusters(), kmeans->get_centroids());

	delete kmeans;
}

extern "C" void free_double_array(double *sil) { free(sil); }

extern "C" void get_stotal(struct config* config, int dim, double *stotal, double **sil, double *obj_func, void *kmeansnew, double **centroids)
{
    *sil = (double*) malloc(10 * sizeof(double));

    // Initialize structure.
    string initial_dataset_str(config->dataset);

    if(!file_exists(initial_dataset_str)){
        cout << "File " << initial_dataset_str << " does not exist." << endl;
        exit(1);
    }

    // Convert centroids to vector<vector<double>>.
    vector<vector<double>> centroids_;
    for (int i = 0; i < 10; i++) {
        vector<double> centroid;
        for (int j = 0; j < dim; j++) {
            centroid.push_back(centroids[i][j]);
        }
        centroids_.push_back(centroid);
    }

    vector <vector<double>> initial_dataset = read_mnist_data_float(initial_dataset_str);

    KMeansEval* kmeans = (KMeansEval*) kmeansnew;

    vector<variant<double, vector<double>>> results = kmeans->silhouette(initial_dataset, centroids_);
    double stotal_ = get<double>(results[0]);
    vector<double> sil_ = get<vector<double>>(results[1]);
    for (int i = 0; i < (int) sil_.size(); i++) {
        (*sil)[i] = sil_[i];
    }

    // Return stotal.
    *stotal = stotal_;

    // Compute objective function.
    *obj_func = compute_objective_function(initial_dataset, kmeans->get_clusters(), centroids_);
}

extern "C" void get_kmeans(struct config* config, void **kmeansnew)
{
    // Initialize structure.
    string encoded_dataset_str(config->encoded_dataset);

    vector <vector<double>> encoded_dataset = read_mnist_data_float(encoded_dataset_str);

    KMeansEval* kmeans = new KMeansEval(encoded_dataset);

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

    kmeans->compute_clusters(10, method, kmean_args);
    
    // Return KMeansEval object.
    *kmeansnew = kmeans;
}

extern "C" void free_kmeans(void *kmeansnew)
{
    KMeansEval *kmeans = (KMeansEval*) kmeansnew;
    delete kmeans;
}

extern "C" void get_centroids(void *kmeansnew, double ***centroids, int *dim) {
    KMeansEval *kmeans = (KMeansEval*) kmeansnew;
    vector<vector<double>> centroids_ = kmeans->get_centroids();
    *centroids = (double**) malloc(centroids_.size() * sizeof(double*));
    for (int i = 0; i < (int) centroids_.size(); i++) {
        (*centroids)[i] = (double*) malloc(centroids_[i].size() * sizeof(double));
        for (int j = 0; j < (int) centroids_[i].size(); j++) {
            (*centroids)[i][j] = centroids_[i][j];
        }
    }

    *dim = centroids_[0].size();
}

extern "C" void free_centroids(double **centroids) {
    for (int i = 0; i < 10; i++) {
        free(centroids[i]);
    }
    free(centroids);
}

// Converts a flattened 2d array given from Python to its original 2d form.
extern "C" void convert_1d_to_2d(double *array, int dim, double ***array2d) {
    *array2d = (double**) malloc(10 * sizeof(double*));
    for (int i = 0; i < 10; i++) {
        (*array2d)[i] = (double*) malloc(dim * sizeof(double));
        for (int j = 0; j < dim; j++) {
            (*array2d)[i][j] = array[i * dim + j];
        }
    }
}