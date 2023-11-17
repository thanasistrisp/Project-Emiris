#include <tuple>
#include <set>
#include <vector>
#include <algorithm>
#include <map>
#include <unordered_set>
#include <set>

#include "gnn.hpp"
#include "lsh.hpp"
#include "defines.hpp"
#include "lp_metric.hpp"
#include "vector_utils.hpp"

using namespace std;

GNN::GNN(int k, const vector<vector<double>> &dataset, int R, int E): dataset(dataset), R(R), E(E)
{
	unordered_multiset<pair<int, double>*, decltype(&hash), decltype(&equal)> neighbors_set(8, &hash, &equal);

	unordered_set<int> unique_indices;

	clock_t start = clock();

	G = new DirectedGraph();
	int k_lsh = 5;
	int L = 5;
	lsh = new LSH(k_lsh, L, dataset.size()/4, w, dataset);
	clock_t end_lsh = clock();
	double elapsed_secs_lsh = double(end_lsh - start) / CLOCKS_PER_SEC;
	cout << "LSH initialization time: " << elapsed_secs_lsh << endl;
	start = clock();
	
	// initialize G
	for (int i = 0; i < (int) dataset.size(); i++) {
		G->add_vertex(i);
	}
	for(int i = 0; i < (int) dataset.size(); i++){
		tuple<vector<int>, vector<double>> neighbors = lsh->query(dataset[i], k, euclidean_distance);
		vector<int> neighbors_indices = get<0>(neighbors);
		vector<double> neighbors_distances = get<1>(neighbors);

		if ((int) neighbors_indices.size() < k) {
			// cout << "LSH query returned " << get<0>(neighbors).size() << " neighbors instead of " << k << endl; // debug.

			// Merge vectors into a set of pairs.
			for(int j = 0; j < (int) neighbors_indices.size(); j++){
				pair<int, double> *p = new pair(neighbors_indices[j], neighbors_distances[j]);
				neighbors_set.insert(p);
			}

			// Add successors of the same predecessor as neighbors.
			add_neighbors_pred(i, neighbors_set, k);

			// If problem persists, add random vertices as neighbors.
			if((int) neighbors_indices.size() < k){
				
				for(int j = 0; j < (int) neighbors_indices.size(); j++){
					unique_indices.insert(neighbors_indices[j]);
				}
				add_neighbors_random(i, neighbors_set, unique_indices, k);
			}

			// Convert set to vector, sort it and then and split it to two vectors.
			vector<pair<int, double>*> neighbors_set_vec(neighbors_set.begin(), neighbors_set.end());
			sort(neighbors_set_vec.begin(), neighbors_set_vec.end(), cmp);
			neighbors_indices.clear();
			neighbors_distances.clear();
			for(int j = 0; j < (int) neighbors_set_vec.size(); j++){
				neighbors_indices.push_back(neighbors_set_vec[j]->first);
				neighbors_distances.push_back(neighbors_set_vec[j]->second);
			}

			// Clean set and pairs.
			for(auto iter = neighbors_set.begin(); iter != neighbors_set.end(); iter++){
				delete *iter;
			}
			neighbors_set.clear();
			unique_indices.clear();

			// cout << "Gathered " << neighbors_indices.size() << " neighbours" << endl; // debug.
		}
		for(int j = 0; j < (int) neighbors_indices.size(); j++){
			G->add_edge(i, neighbors_indices[j], neighbors_distances[j]);
		}
		// cout << "LSH query " << i << endl;
	}

	clock_t end = clock();
	double elapsed_secs = double(end - start) / CLOCKS_PER_SEC;
	cout << "GNN initialization time: " << elapsed_secs << endl;
}

GNN::~GNN()
{
	delete G;
	delete lsh;
}

void GNN::add_neighbors_pred(int index, unordered_multiset<pair<int, double>*, decltype(&hash), decltype(&equal)>& neighbors, int k)
{
	vector<int> pred = G->get_predecessors(index, 1);
	double distance;

	if((int) pred.size() > 0){
		vector<Vertex*> pred_successors = G->get_successors(pred[0]);
		
		for(int i = 0; i < (int) pred_successors.size(); i++){
			int ps_index = pred_successors[i]->get_index();
			distance = euclidean_distance(dataset[index], dataset[ps_index]);
			pair<int, double>* p = new pair(ps_index, distance);
			neighbors.insert(p);

			if((int) neighbors.size() == k){
				break;
			}
		}
	}
}

void GNN::add_neighbors_random(int index, unordered_multiset<pair<int, double>*, decltype(&hash), decltype(&equal)>& neighbors, unordered_set<int>& unique_indices, int k)
{
	double distance;

	while((int) neighbors.size() < k){
		int r_index = rand() % dataset.size();
		if(unique_indices.find(r_index) != unique_indices.end()){
			continue;
		}
		unique_indices.insert(r_index);

		distance = euclidean_distance(dataset[index], dataset[r_index]);
		pair<int, double>* p = new pair(r_index, distance);
		neighbors.insert(p);
	}
}

tuple<vector<int>, vector<double>> GNN::query(const vector<double>& q, unsigned int N,
                                              double (*distance)(const vector<double>&, const vector<double>&))
{
	auto cmp = [](pair<double, int> left, pair<double, int> right) { return left.first < right.first; };
	multiset<pair<double, int>, decltype(cmp)> S(cmp);

	unordered_set<int> unique_indices;

	for (int i = 0; i < R; i++) {
		int y0, y1, y0_dist, y1_dist;
		unordered_set<int> visited;
		y0 = rand() % dataset.size();
		y0_dist = distance(q, dataset[y0]);
		visited.insert(y0);
		while (true) {
			vector<Vertex*> neighbors = G->get_successors(y0, E);
			y1 = neighbors[0]->get_index();
			y1_dist = distance(q, dataset[y1]);
			// check if y1 is in S
			if (unique_indices.find(y1) == unique_indices.end()) {
				S.insert(make_pair(y1_dist, y1));
				unique_indices.insert(y1);
			}
			for (int j = 1; j < (int) neighbors.size(); j++) {
				int temp = neighbors[j]->get_index();
				double temp_dist = distance(q, dataset[temp]);
				// check if temp is in S
				if (unique_indices.find(temp) == unique_indices.end()) {
					S.insert(make_pair(temp_dist, temp));
					unique_indices.insert(temp);
				}
				if (temp_dist < y1_dist) {
					y1 = temp;
					y1_dist = temp_dist;
				}
			}

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
	vector<int> S_N;
	vector<double> S_N_dist;
	uint i = 0;
	for (auto it = S.begin(); it != S.end() && i < N; it++, i++) {
		S_N.push_back(it->second);
		S_N_dist.push_back(it->first);
	}
	return make_tuple(S_N, S_N_dist);
}