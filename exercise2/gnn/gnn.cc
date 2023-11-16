#include "gnn.hpp"
#include "lsh.hpp"
#include "defines.hpp"
#include "lp_metric.hpp"
#include <tuple>
#include <set>
#include <vector>
#include <algorithm>
#include <map>

using namespace std;

GNN::GNN(int k, const vector<vector<double>> &dataset, int R, int E): dataset(dataset), R(R), E(E)
{
	G = new DirectedGraph();
	int k_lsh = 4;
	int L = 5;
	lsh = new LSH(k_lsh, L, dataset.size()/4, w, dataset);
	
	// initialize G
	for (int i = 0; i < (int)dataset.size(); i++) {
		G->add_vertex(i);
	}
	for(int i = 0; i < (int)dataset.size(); i++){
		tuple<vector<int>, vector<double>> neighbors = lsh->query(dataset[i], k, euclidean_distance);
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
	delete lsh;
}

tuple<vector<int>, vector<double>> GNN::query(const vector<double>& q, unsigned int N,
                                              double (*distance)(const vector<double>&, const vector<double>&))
{
	multimap<double, int> S;
	for (int i = 0; i < R; i++) {
		int y0, y1, y0_dist, y1_dist;
		y0 = rand() % dataset.size();
		y0_dist = distance(q, dataset[y0]);
		while (true) {
			vector<Vertex*> neighbors = G->get_successors(y0, E);
			for (int j = 0; j < (int)neighbors.size(); j++) {
				S.insert(make_pair(neighbors[j]->get_distance(), neighbors[j]->get_index()));
			}

			// y1 is minimum from neighbors
			y1 = neighbors[0]->get_index();
			y1_dist = neighbors[0]->get_distance();
			for (int j = 1; j < (int)neighbors.size(); j++) {
				if (neighbors[j]->get_distance() < y1_dist) {
					y1 = neighbors[j]->get_index();
					y1_dist = neighbors[j]->get_distance();
				}
			}

			// if we reached a local minimum, break
			if (y1_dist < y0_dist) {
				y0 = y1;
			}
			else {
				break;
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