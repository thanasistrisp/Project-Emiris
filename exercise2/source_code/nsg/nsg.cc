#include "nsg.hpp"
#include "vector_utils.hpp"
#include "approximate_knn_graph.hpp"
#include "generic_search.hpp"
#include "depth_first_search.hpp"
#include "brute_force.hpp"

using namespace std;

NSG::NSG(const std::vector<std::vector<double>> &dataset, int total_candidates, int m, int k) : dataset(dataset)
{
	// Create Approximate kNN graph.
	ApproximateKNNGraph *knn = new ApproximateKNNGraph(dataset, k);
	DirectedGraph *knn_graph = knn->get_graph();
	G = new DirectedGraph();
	for (int i = 0; i < (int) dataset.size(); i++) {
		G->add_vertex(i);
	}

	// Calculate the centroid of the dataset.
    vector<double> dataset_centroid = vector<double>(dataset[0].size(), 0.0);
    for(int i = 0; i < (int) dataset.size(); i++){
        dataset_centroid = vector_addition(dataset_centroid, dataset[i]);
    }
    dataset_centroid = vector_scalar_mult(dataset_centroid, 1 / dataset.size());

	// R is a random node.
	int r = rand() % dataset.size();

	// n is navigating node from generic search.
	tuple<vector<int>, vector<double>> neighbors = generic_search_on_graph(*knn_graph, dataset, r, dataset_centroid, total_candidates, 1, distance);
	navigating_node = get<0>(neighbors)[0];

	// For all node v in G.
	for(int v = 0; v < (int) dataset.size(); v++){
		vector<double> v_query = dataset[v];
		deque<pair<int, double>> E = generic_search_on_graph_checked(*knn_graph, dataset, navigating_node, v_query, total_candidates, distance);
		
		unordered_set<int> R;
		// p0 is the closest node to v in E.
		int p0 = E.front().first;
		// Add p0 to R.
		R.insert(p0);
		while (!E.empty() && (int) R.size() < m) {
			// Take first node from E and pop it.
			int p = E.front().first;
			E.pop_front();
			// For all nodes in R.
			double pv = distance(dataset[p], dataset[v]);
			bool condition = true;
			for (int r : R) {
				// If edge pv conflicts with edge pr, break.
				double rv = distance(dataset[r], dataset[v]);
				double pr = distance(dataset[p], dataset[r]);
				if (pv > pr && pv > rv) {
					condition = false;
					break;
				}
			}
			// If no conflict occurs, add p to R.
			if (condition)
				R.insert(p);
		}
		for (int r : R)
			if (r != v)
				G->add_edge(v, r);
	}

	DirectedGraph *dfs_spanning_tree;
	unordered_set<int> dfs_checked;
	while(true){
		// Build a tree with edges in NSG from root n with DFS.
		tie(dfs_spanning_tree, dfs_checked) = depth_first_search(*G, navigating_node);

		if(dfs_checked.size() == dataset.size()){
			delete dfs_spanning_tree;
			break;
		}

		// If not all nodes linked to the tree then.
		int i;
		for(i = 0; i < (int) dataset.size(); i++){	
			if(dfs_checked.find(i) == dfs_checked.end()){
				break;
			}
		}

		// Add an edge between one of the out-of-tree nodes and
		// its closest in-tree neighbor.
		neighbors = generic_search_on_graph(*dfs_spanning_tree, dataset, navigating_node, dataset[i], total_candidates, 1, euclidean_distance);
		int closest_neighbor = get<0>(neighbors)[0];

		G->add_edge(closest_neighbor, i);
		G->add_edge(i, closest_neighbor);

		delete dfs_spanning_tree;
	}
	delete knn;
}

tuple<vector<int>, vector<double>> NSG::query(const vector<double>& q, unsigned int N, unsigned int L)
{
	return generic_search_on_graph(*G, dataset, navigating_node, q, L, N, distance);
}


NSG::NSG(const std::vector<std::vector<double>> &dataset, DirectedGraph *G) : dataset(dataset), G(G)
{

}

NSG::~NSG()
{
	delete G;
}