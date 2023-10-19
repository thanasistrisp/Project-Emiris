#include <vector>

#include "kmeans.h"

KMeans::KMeans(std::vector<std::vector<double>> &dataset, int k) : dataset(dataset), k(k)
{
    
}

KMeans::~KMeans()
{

}

void KMeans::assign()
{

}

void KMeans::update()
{

}

void KMeans::compute_clusters(int k, update_method method, std::vector<int> method_args, const std::vector<double>&)
{

}

std::vector<std::vector<double>> KMeans::get_centroids() const
{
    return centroids;
}

std::vector<std::vector<int>> KMeans::get_clusters() const
{
    return clusters;
}