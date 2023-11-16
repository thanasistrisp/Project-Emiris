#include "gnn.hpp"
#include "lsh.hpp"
#include "defines.hpp"
#include "lp_metric.hpp"
#include <tuple>
#include <set>
#include <vector>
#include <algorithm>
#include <map>
#include <unordered_set>

using namespace std;

GNN::GNN(int k, const vector<vector<double>> &dataset, int R, int E): dataset(dataset), R(R), E(E)
{
	clock_t start = clock();

	G = new DirectedGraph();
	int k_lsh = 4;
	int L = 5;
	lsh = new LSH(k_lsh, L, dataset.size()/4, w, dataset);
	
	// initialize G
	for (int i = 0; i < (int)dataset.size(); i++) {
		G->add_vertex(i);
	}
	for(int i = 0; i < (int)dataset.size(); i++){
		tuple<vector<int>, vector<double>> neighbors = lsh->query(dataset[i], k, euclidean_distance, false);
		vector<int> neighbors_indices = get<0>(neighbors);
		vector<double> neighbors_distances = get<1>(neighbors);
		for(int j = 0; j < (int)neighbors_indices.size(); j++){
			G->add_edge(i, neighbors_indices[j], neighbors_distances[j]);
		}
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

tuple<vector<int>, vector<double>> GNN::query(const vector<double>& q, unsigned int N,
                                              double (*distance)(const vector<double>&, const vector<double>&))
{
	auto cmp = [](pair<double, int> left, pair<double, int> right) { return left.first < right.first; };
	set<pair<double, int>, decltype(cmp)> S(cmp);

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
			for (int j = 1; j < (int)neighbors.size(); j++) {
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