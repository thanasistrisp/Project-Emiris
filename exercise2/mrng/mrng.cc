#include <tuple>
#include <set>
#include <vector>
#include <algorithm>
#include <map>
#include <unordered_set>

#include "mrng.hpp"
#include "lsh.hpp"
#include "defines.hpp"
#include "set_utils.hpp"

using namespace std;

MRNG::MRNG(int k, const vector<vector<double>> &dataset, int R, int E): dataset(dataset), R(R), E(E)
{
	k++;
	int k_lsh = 5;
	int L = 5;
	lsh = new LSH(k_lsh, L, dataset.size()/4, w, dataset);
	// initialize graph
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
		cout << i << endl;
		// Rp is S - {p}
		for (int r : S) {
			if (r != p) {
				Rp->insert(r);
			}
		}
		find_neighbors_with_min_distance(p, Lp);
		bool condition = true;
		// for r in Rp and r not in Lp
		for (int r : *Rp) {
			if (Lp->find(r) == Lp->end()) {
				for (int t : *Lp) {
					// if pr longest edge in triangle prt
					double pr = distance(dataset[p], dataset[r]);
					double pt = distance(dataset[p], dataset[t]);
					double rt = distance(dataset[r], dataset[t]);
					
					if (pr > pt && pr > rt)
					{
						condition = false;
						break;
					}
				}
				if (condition)
				{
					Lp->insert(r);
				}
			}
		}
        // for each Lp add edge (p, l)
        for (int l : *Lp) {
            G->add_edge(p, l);
        }
		Lp->clear();
		Rp->clear();
	}
	delete Rp;
	delete Lp;
}

MRNG::~MRNG()
{
	delete G;
	delete lsh;
}

tuple<vector<int>, vector<double>> MRNG::query(const vector<double>& q, unsigned int N,
                                              double (*distance)(const vector<double>&, const vector<double>&))
{
    vector<double> v1 = vector<double>();
    vector<double> v2 = vector<double>();
    distance(v1, v2);
    cout << q[0];
    N++;
	return make_tuple(vector<int>(), vector<double>());
}

void MRNG::find_neighbors_with_min_distance(int p, unordered_set<int> *Lp)
{
	// by using lsh, start with k=5 and increase k by 5 till we find neighbors with different distances
	int k = 5;
	vector<int> neighbors_indices;
	vector<double> neighbors_distances;
	tuple<vector<int>, vector<double>> neighbors = lsh->query(dataset[p], k, distance, false);
	neighbors_indices = get<0>(neighbors);
	neighbors_distances = get<1>(neighbors);
	while (neighbors_distances[0] == (int) neighbors_distances[neighbors_distances.size() - 1]) {
		k += 5;
		neighbors = lsh->query(dataset[p], k, distance);
		neighbors_indices = get<0>(neighbors);
		neighbors_distances = get<1>(neighbors);
	}
	// add neighbors with same distance to Lp
	for (int i = 0; i < (int) neighbors_indices.size(); i++) {
		if (neighbors_distances[i] == neighbors_distances[0]) {
			Lp->insert(neighbors_indices[i]);
		}
	}
}