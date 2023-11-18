#include <tuple>
#include <set>
#include <vector>
#include <algorithm>
#include <map>
#include <unordered_set>

#include "mrng.hpp"
#include "lsh.hpp"
#include "defines.hpp"
#include "lp_metric.hpp"
#include "vector_utils.hpp"
#include "brute_force.hpp"
#include "generic_search.hpp"

using namespace std;

MRNG::MRNG(int k, const vector<vector<double>> &dataset, int R, int E): dataset(dataset), R(R), E(E)
{
	k++;
    R++;
    E++;
    set_navigating_node();
}

MRNG::~MRNG()
{
	
}

void MRNG::set_navigating_node()
{
    // Calculate the centroid of the dataset.
    vector<double> dataset_centroid = vector<double>(dataset[0].size(), 0.0);
    for(int i = 0; i < (int) dataset.size(); i++){
        dataset_centroid = vector_addition(dataset_centroid, dataset[i]);
    }
    dataset_centroid = vector_scalar_mult(dataset_centroid, 1 / dataset.size());

    // Treat it as a query, find its nearest neighbor by brute force.
    vector<int> indices;
    vector<double> distances;
    tie(indices, distances) = brute_force(dataset, dataset_centroid, 1, euclidean_distance);
    navigating_node = indices[0];
}

tuple<vector<int>, vector<double>> MRNG::query(const vector<double>& q, unsigned int N,
                                              double (*distance)(const vector<double>&, const vector<double>&))
{
    int l = 20;
    return generic_search_on_graph(*G, dataset, navigating_node, q, l, N, euclidean_distance);
}