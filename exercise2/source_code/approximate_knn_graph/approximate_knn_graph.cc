#include <tuple>
#include <vector>
#include <algorithm>
#include <unordered_set>
#include <set>

#include "approximate_knn_graph.hpp"
#include "lsh.hpp"
#include "lp_metric.hpp"
#include "vector_utils.hpp"
#include "set_utils.hpp"

#ifdef NEW
#include "defines_latent_space.hpp"
#else
#include "defines_initial_space.hpp"
#endif

using namespace std;

ApproximateKNNGraph::ApproximateKNNGraph(const vector<vector<double>> &dataset, int k): dataset(dataset)
{
	unordered_multiset<pair<int, double>*, decltype(&set_hash), decltype(&set_equal)> neighbors_set(8, &set_hash, &set_equal);
	unordered_set<int> unique_indices;

	G = new DirectedGraph();
	// clock_t start = clock();
	lsh = new LSH(k_lsh, L, table_size, window_size, dataset);
	// clock_t end_lsh = clock();
	// double elapsed_secs_lsh = double(end_lsh - start) / CLOCKS_PER_SEC;
	// cout << "LSH initialization time: " << elapsed_secs_lsh << endl;
	
	// Initialize G.
	for (int i = 0; i < (int) dataset.size(); i++) {
		G->add_vertex(i);
	}
	for(int i = 0; i < (int) dataset.size(); i++){
		tuple<vector<int>, vector<double>> neighbors = lsh->query(dataset[i], k, distance, false);
		vector<int> neighbors_indices = get<0>(neighbors);

		if ((int) neighbors_indices.size() < k) {
			// Merge vectors into a set of pairs.
			for(int j = 0; j < (int) neighbors_indices.size(); j++){
				pair<int, double> *p = new pair(neighbors_indices[j], distance(dataset[i], dataset[neighbors_indices[j]]));
				neighbors_set.insert(p);
				unique_indices.insert(p->first);
			}

			// Add successors of the same predecessor as neighbors.
			add_neighbors_pred(i, neighbors_set, unique_indices, k);

			// If problem persists, add random vertices as neighbors.
			if((int) neighbors_set.size() < k){
				add_neighbors_random(i, neighbors_set, unique_indices, k);
			}

			// Convert set to vector, sort it and then and split it to two vectors.
			vector<pair<int, double>*> neighbors_set_vec(neighbors_set.begin(), neighbors_set.end());
			sort(neighbors_set_vec.begin(), neighbors_set_vec.end(), set_cmp);
			neighbors_indices.clear();
			for(int j = 0; j < (int) neighbors_set_vec.size(); j++){
				neighbors_indices.push_back(neighbors_set_vec[j]->first);
			}

			// Clean set and pairs.
			for(auto iter = neighbors_set.begin(); iter != neighbors_set.end(); iter++){
				delete *iter;
			}
			neighbors_set.clear();
			unique_indices.clear();
		}
		G->add_edge(i, neighbors_indices);
	}
}

ApproximateKNNGraph::~ApproximateKNNGraph()
{
	delete G;
	delete lsh;
}

void ApproximateKNNGraph::add_neighbors_pred(int index, unordered_multiset<pair<int, double>*, decltype(&set_hash), decltype(&set_equal)>& neighbors, unordered_set<int>& unique_indices, int k)
{
	vector<int> pred = G->get_predecessors(index, 1);
	double dist;

	if((int) pred.size() > 0){
		vector<int> pred_successors = G->get_successors(pred[0]);
		
		for(int i = 0; i < (int) pred_successors.size(); i++){
			int ps_index = pred_successors[i];

			if(ps_index == index || unique_indices.find(ps_index) != unique_indices.end()){
				continue;
			}
			unique_indices.insert(ps_index);

			dist = distance(dataset[index], dataset[ps_index]);
			pair<int, double>* p = new pair(ps_index, dist);
			neighbors.insert(p);

			if((int) neighbors.size() == k){
				break;
			}
		}
	}
}

void ApproximateKNNGraph::add_neighbors_random(int index, unordered_multiset<pair<int, double>*, decltype(&set_hash), decltype(&set_equal)>& neighbors, unordered_set<int>& unique_indices, int k)
{
	double dist;

	while((int) neighbors.size() < k){
		int r_index = rand() % dataset.size();
		if(r_index == index || unique_indices.find(r_index) != unique_indices.end()){
			continue;
		}
		unique_indices.insert(r_index);

		dist = distance(dataset[index], dataset[r_index]);
		pair<int, double>* p = new pair(r_index, dist);
		neighbors.insert(p);
	}
}

// GNNS algorithm.
tuple<vector<int>, vector<double>> ApproximateKNNGraph::query(const vector<double>& q, unsigned int N, unsigned int E, unsigned int R)
{
	auto cmp = [](pair<double, int> left, pair<double, int> right) { return left.first < right.first; };
	multiset<pair<double, int>, decltype(cmp)> S(cmp);

	unordered_set<int> unique_indices;

	for (uint i = 0; i < R; i++) {
		int y0, y1;
		double y0_dist, y1_dist;
		unordered_set<int> visited;

		// Y_0: a random point (uniformly) over D.
		y0 = rand() % dataset.size();
		y0_dist = distance(q, dataset[y0]);
		visited.insert(y0);
		while (true) {
			// Y_t = argmin_{Y \in N(Y_{t-1}, E, G)} \delta (Y, Q).
			vector<int> neighbors = G->get_successors(y0, E);
			y1 = neighbors[0];
			y1_dist = distance(q, dataset[y1]);
			// Check if y1 is in S.
			if (unique_indices.find(y1) == unique_indices.end()) {
				S.insert(make_pair(y1_dist, y1));
				unique_indices.insert(y1);
			}
			// S = S U N(Y_{t-1}, E, G).
			for (int j = 1; j < (int) neighbors.size(); j++) {
				int temp = neighbors[j];
				double temp_dist = distance(q, dataset[temp]);
				// Check if temp is in S.
				if (unique_indices.find(temp) == unique_indices.end()) {
					S.insert(make_pair(temp_dist, temp));
					unique_indices.insert(temp);
				}
				if (temp_dist < y1_dist) {
					y1 = temp;
					y1_dist = temp_dist;
				}
			}

			// If node is better than its new neighbors, end search (local optimal).
			if (y0_dist <= y1_dist) {
				break;
			}
			else {
				if (visited.find(y1) != visited.end()) {
					break;
				}
				else {
					y0 = y1;
					y0_dist = y1_dist;
					visited.insert(y0);
				}
			}
		}
	}
	// Return N points in S with smallest distances.
	vector<int> S_N;
	vector<double> S_N_dist;
	uint i = 0;
	for (auto it = S.begin(); it != S.end() && i < N; it++, i++) {
		S_N.push_back(it->second);
		S_N_dist.push_back(it->first);
	}
	return make_tuple(S_N, S_N_dist);
}