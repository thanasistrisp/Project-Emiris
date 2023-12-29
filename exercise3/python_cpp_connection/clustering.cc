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

#include "brute_force.hpp"

#include "lsh.hpp"
#include "hypercube.hpp"
#include "kmeans.hpp"
#include "approximate_knn_graph.hpp"
#include "nsg.hpp"
#include "mrng.hpp"

using namespace std;
using std::cout;

#define cout if(0) cout // Comment this line to enable printing.

vector<variant<double, int>> helper_arg_cluster(void *structure, vector<variant<int, bool, string>> &params)
{
    string method_str = get<string>(params[0]);
    int L = get<int>(params[1]);
    int k_lsh = get<int>(params[2]);
    int M = get<int>(params[3]);
    int k_hypercube = get<int>(params[4]);
    int probes = get<int>(params[5]);
    update_method method;
    if (method_str == "classic") {
        method = CLASSIC;
    }
    else if (method_str == "lsh") {
        method = REVERSE_LSH;
    }
    else {
        method = REVERSE_HYPERCUBE;
    }
    tuple<int, int, int, int, int> config = make_tuple(L, k_lsh, M, k_hypercube, probes);
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

    return {clustering_time, stotal};
}

extern "C" void get_kmeans_results(const char *input, const char *method,
                                    int L, int k_lsh, int M, int k_hypercube, int probes,
									double *clustering_time, double *stotal)
{
	string input_str(input);
	string method_str(method);

	cout << "Read MNIST data" << endl;
	vector <vector<double>> dataset = read_mnist_data(input_str);
	KMeans *kmeans = new KMeans(dataset);
	cout << "Done" << endl;

	// Return time, stotal.
	vector<variant<int, bool, string>> params = {method_str, L, k_lsh, M, k_hypercube, probes};
	vector<variant<double, int>> results = helper_arg_cluster(kmeans, params);
	*clustering_time = get<double>(results[0]);
	*stotal = get<double>(results[1]);

	delete kmeans;
}