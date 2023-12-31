#include <tuple>
#include <set>
#include <vector>
#include <algorithm>
#include <map>
#include <unordered_set>

#include "mrng.hpp"
#include "lsh.hpp"
#include "set_utils.hpp"
#include "lp_metric.hpp"
#include "vector_utils.hpp"
#include "brute_force.hpp"
#include "generic_search.hpp"

#ifdef NEW
#include "defines_latent_space.hpp"
#else
#include "defines_initial_space.hpp"
#endif

using namespace std;

MRNG::MRNG(const vector<vector<double>> &dataset): dataset(dataset)
{
	// clock_t start = clock();
	lsh = new LSH(k_lsh, L, table_size, window_size, dataset);
	// clock_t end_lsh = clock();
	// double elapsed_secs_lsh = double(end_lsh - start) / CLOCKS_PER_SEC;
	// cout << "LSH initialization time: " << elapsed_secs_lsh << endl;
	// Initialize graph.
	G = new DirectedGraph();
	unordered_set<int> S;
	for (int i = 0; i < (int) dataset.size(); i++) {
		G->add_vertex(i);
		S.insert(i);
	}
	int i = 0;
	unordered_set<int> *Rp =  new unordered_set<int>();
	unordered_set<int> *Lp = new unordered_set<int>();
	for (int p: S) {
		i++;
		// cout << i << endl;
		// Rp is S - {p}.
		for (int r : S) {
			if (r != p) {
				Rp->insert(r);
			}
		}
		find_neighbors_with_min_distance(p, Lp);
		// For r in Rp and r not in Lp.
		for (int r : *Rp) {
			bool condition = true;
			if (Lp->find(r) == Lp->end()) {
				double pr = distance(dataset[p], dataset[r]);
				for (int t : *Lp) {
					// If pr longest edge in triangle prt.
					double pt = distance(dataset[p], dataset[t]);
					double rt = distance(dataset[r], dataset[t]);
					
					if (pr > pt && pr > rt) {
						condition = false;
						break;
					}
				}
				// If pr not the longest edge in triange prt \forall t.
				if (condition)
					Lp->insert(r);
			}
		}
        // For each Lp add edge (p, l).
        for (int l : *Lp)
            G->add_edge(p, l);
		Lp->clear();
		Rp->clear();
	}
	delete Rp;
	delete Lp;
    set_navigating_node();
}

MRNG::MRNG(const std::vector<std::vector<double>> &dataset, DirectedGraph *G)
: dataset(dataset), G(G)
{
	set_navigating_node();
}

MRNG::~MRNG()
{
	delete G;
	delete lsh;
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
    tie(indices, distances) = brute_force(dataset, dataset_centroid, 1, distance);
    navigating_node = indices[0];
}

tuple<vector<int>, vector<double>> MRNG::query(const vector<double>& q, unsigned int N, unsigned int l)
{
    return generic_search_on_graph(*G, dataset, navigating_node, q, l, N, distance);
}

void MRNG::find_neighbors_with_min_distance(int p, unordered_set<int> *Lp)
{
	// Use lsh, start with k = 5 and increase k by 5 till we find neighbors with different distances.
	int k = 5;
	vector<int> neighbors_indices;
	vector<double> neighbors_distances;
	tuple<vector<int>, vector<double>> neighbors = lsh->query(dataset[p], k, distance, true);
	neighbors_indices = get<0>(neighbors);
	neighbors_distances = get<1>(neighbors);
	
	// In case LSH returns no neighbors, pick a random one.
	if((int) neighbors_indices.size() == 0){

		// cout << "LSH returned no neighbors, adding a random neighbor instead" << endl;

		int r_index = rand() % dataset.size();
		while(r_index != p){
			r_index = rand() % dataset.size();
		}
		neighbors_indices.push_back(r_index);
		neighbors_distances.push_back(distance(dataset[p], dataset[r_index]));
	}
	else{
		while (neighbors_distances[0] == (int) neighbors_distances[neighbors_distances.size() - 1]) {
			k += 5;
			neighbors = lsh->query(dataset[p], k, distance, true);
			neighbors_indices = get<0>(neighbors);
			neighbors_distances = get<1>(neighbors);
		}
	}
	// Add neighbors with same distance to Lp.
	for (int i = 0; i < (int) neighbors_indices.size(); i++) {
		if (neighbors_distances[i] == neighbors_distances[0]) {
			Lp->insert(neighbors_indices[i]);
		}
	}
}