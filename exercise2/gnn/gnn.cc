#include "gnn.hpp"
#include "hypercube.hpp"
#include "defines.hpp"
#include "lp_metric.hpp"
#include <tuple>
#include <set>
#include <vector>
#include <algorithm>
#include <iostream>
#include <map>
#include <unordered_set>

using namespace std;

GNN::GNN(int k, const vector<vector<double>> &dataset, int R, int E): dataset(dataset), R(R), E(E)
{
	G = new DirectedGraph();
	cube = new hypercube(dataset, 7, 100, 100, euclidean_distance);
	
	// initialize G
	for (int i = 0; i < (int)dataset.size(); i++) {
		G->add_vertex(i);
	}
	for(int i = 0; i < (int)dataset.size(); i++){
		vector<double> q = dataset[i];
		// convert to q_proj
		vector<int> q_proj = cube->calculate_q_proj(q);
		tuple<vector<int>, vector<double>> neighbors = cube->query_n_nearest_neighbors(dataset[i], q_proj, k);
		vector<int> neighbors_indices = get<0>(neighbors);
		vector<double> neighbors_distances = get<1>(neighbors);
		for(int j = 0; j < (int)neighbors_indices.size(); j++){
			G->add_edge(i, neighbors_indices[j], neighbors_distances[j]);
		}
	}
}

GNN::~GNN()
{
	delete G;
	delete cube;
}

tuple<vector<int>, vector<double>> GNN::query(const vector<double>& q, unsigned int N,
                                              double (*distance)(const vector<double>&, const vector<double>&))
{
	multimap<double, int> S;
	for (int i = 0; i < R; i++) {
		int y0, y1, y0_dist, y1_dist;
		unordered_set<int> visited;
		y0 = rand() % dataset.size();
		y0_dist = distance(q, dataset[y0]);
		visited.insert(y0);
		while (true) {
			vector<Vertex*> neighbors = G->get_successors(y0, E);
			for (int j = 0; j < (int)neighbors.size(); j++) {
				S.insert(make_pair(distance(q, dataset[neighbors[j]->get_index()]), neighbors[j]->get_index()));
			}

			// y1 is minimum from neighbors
			y1 = neighbors[0]->get_index();
			y1_dist = distance(q, dataset[y1]);
			for (int j = 1; j < (int)neighbors.size(); j++) {
				if (distance(q, dataset[neighbors[j]->get_index()]) < y1_dist) {
					y1 = neighbors[j]->get_index();
					y1_dist = distance(q, dataset[y1]);
				}
			}

			if (y0_dist < y1_dist) {
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
	for (int i = 0; i < (int)N; i++) {
		S_N.push_back(S.begin()->second);
		S_N_dist.push_back(S.begin()->first);
	}
	return make_tuple(S_N, S_N_dist);
}