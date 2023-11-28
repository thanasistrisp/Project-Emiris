#include "nsg.hpp"
#include "vector_utils.hpp"
#include "generic_search.hpp"
#include "approximate_knn_graph.hpp"

using namespace std;

NSG::NSG(const std::vector<std::vector<double>> &dataset, int total_candidates, int m) : dataset(dataset)
{
	// create knn graph
	ApproximateKNNGraph *knn_graph = new ApproximateKNNGraph(dataset, total_candidates);
	DirectedGraph *G = knn_graph->get_graph();
	DirectedGraph *NSG_graph = new DirectedGraph();
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
}


NSG::NSG(const std::vector<std::vector<double>> &dataset, DirectedGraph *G) : dataset(dataset), G(G)
{

}

NSG::~NSG()
{
	
}