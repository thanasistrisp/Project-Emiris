#include "directed_graph.hpp"
#include "lp_metric.hpp"

class NSG
{
private:
	const std::vector<std::vector<double>> &dataset;
	DirectedGraph *G;
	int navigating_node;

	void set_navigating_node();
public:
	NSG(const std::vector<std::vector<double>> &dataset, int total_candidates, int m, int k);
	NSG(const std::vector<std::vector<double>> &dataset, DirectedGraph *G);
	~NSG();

	std::tuple<std::vector<int>, std::vector<double>> query(const std::vector<double>&, unsigned int N, unsigned int L);

	static constexpr double (*distance)(const std::vector<double>&, const std::vector<double>&) = euclidean_distance;
	DirectedGraph *get_graph() const { return G; }
};