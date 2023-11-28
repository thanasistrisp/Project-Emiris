#include "nsg.hpp"
#include "vector_utils.hpp"
#include "approximate_knn_graph.hpp"
#include "generic_search.hpp"
#include "depth_first_search.hpp"
#include "brute_force.hpp"

using namespace std;

NSG::NSG(const std::vector<std::vector<double>> &dataset, int total_candidates, int m) : dataset(dataset)
{
	// create knn graph
	ApproximateKNNGraph *knn_graph = new ApproximateKNNGraph(dataset, total_candidates);
	DirectedGraph *G = knn_graph->get_graph();
	NSG_graph = new DirectedGraph();
	for (int i = 0; i < (int) dataset.size(); i++) {
		NSG_graph->add_vertex(i);
	}

	// Calculate the centroid of the dataset.
    vector<double> dataset_centroid = vector<double>(dataset[0].size(), 0.0);
    for(int i = 0; i < (int) dataset.size(); i++){
        dataset_centroid = vector_addition(dataset_centroid, dataset[i]);
    }
    dataset_centroid = vector_scalar_mult(dataset_centroid, 1 / dataset.size());

	// r is a random node
	int r = rand() % dataset.size();

	// n is navigating node from generic search
	tuple<vector<int>, vector<double>> neighbors = generic_search_on_graph(*G, dataset, r, dataset_centroid, total_candidates, 1, distance);
	int n = get<0>(neighbors)[0];

	// for all node v in G
	for(int v = 0; v < (int) dataset.size(); v++){
		vector<double> v_query = dataset[v];
		deque<pair<int, double>> E = generic_search_on_graph_checked(*G, dataset, n, v_query, total_candidates, distance);
		
		unordered_set<int> R;
		// p0 is the closest node to v in E
		int p0 = E.front().first;
		// add p0 to R
		R.insert(p0);
		while (!E.empty() && (int) R.size() < m) {
			// take first node from E and pop it
			int p = E.front().first;
			E.pop_front();
			// for all nodes in R
			double pv = distance(dataset[p], dataset[v]);
			bool condition = true;
			for (int r : R) {
				// if edge pv conflicts with edge pr break
				double rv = distance(dataset[r], dataset[v]);
				double pr = distance(dataset[p], dataset[r]);
				if (pv > pr && pv > rv) {
					condition = false;
					break;
				}
			}
			// if no conflict add p to R
			if (condition)
				R.insert(p);
		}
		for (int r : R)
			NSG_graph->add_edge(v, r);
	}

	while(true){
		// Build a tree with edges in NSG from root n with DFS.
		DirectedGraph *dfs_spanning_tree = depth_first_search(*NSG_graph, n);

		if(dfs_spanning_tree->get_number_of_nodes() == dataset.size()){
			break;
		}

		// If not all nodes linked to the tree then.
		int i;
		for(i = 0; i < (int) dataset.size(); i++){	
			vector<int> predecessors = dfs_spanning_tree->get_predecessors(i);
			if(predecessors.size() == 0){
				break;
			}
		}

		// Add an edge between one of the out-of-tree nodes and
		// its closest in-tree neighbor.
		neighbors = generic_search_on_graph(*dfs_spanning_tree, dataset, n, dataset[i], total_candidates, 1, euclidean_distance);
		int closest_neighbor = get<0>(neighbors)[0];
		
		NSG_graph->add_edge(closest_neighbor, i);
	}

	set_navigating_node();
}

void NSG::set_navigating_node()
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

tuple<vector<int>, vector<double>> NSG::query(const vector<double>& q, unsigned int N, unsigned int L)
{
	return generic_search_on_graph(*NSG_graph, dataset, navigating_node, q, L, N, distance);
}


NSG::NSG(const std::vector<std::vector<double>> &dataset, DirectedGraph *G) : dataset(dataset), NSG_graph(G)
{

}

NSG::~NSG()
{
	
}