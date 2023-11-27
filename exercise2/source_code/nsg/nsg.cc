#include "nsg.hpp"

NSG::NSG(const std::vector<std::vector<double>> &dataset) : dataset(dataset)
{

}

NSG::NSG(const std::vector<std::vector<double>> &dataset, DirectedGraph *G) : dataset(dataset), G(G)
{

}

NSG::~NSG()
{

}