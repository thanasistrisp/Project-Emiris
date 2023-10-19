#include <vector>

#include "kmeans.h"

KMeans::KMeans(std::vector<std::vector<double>> &dataset, int k) : dataset(dataset), k(k)
{
    
}

KMeans::~KMeans()
{

}

void KMeans::assign_lloyds(int index)
{

}

void KMeans::assign_lsh(int index)
{

}

void KMeans::assign_hypercube(int index)
{

}

void KMeans::update()
{

}

void KMeans::compute_clusters(int k, update_method method, std::vector<int> method_args, const std::vector<double>&)
{
    // Set assigning function depending on method.
    void (KMeans::*assign)(int);
    if(method == CLASSIC){
        assign = &KMeans::assign_lloyds;
    }
    else if(method == LSH){
        assign = &KMeans::assign_lsh;
    }
    else {
        assign = &KMeans::assign_hypercube;
    }

    // Initialize centroids using KMeans++ algorithm.
    kmeanspp();
    while(true){
        for(int i = 0; i < (int) dataset.size(); i++){
            // Assign point to cluster and apply MacQueen's update rule.
            (this->*assign)(i);
            update();
        }
        update();
    }
}

/* todo: add check for unchanged cluster centroids => end of algorithm */

std::vector<std::vector<double>> KMeans::get_centroids() const
{
    return centroids;
}

std::vector<std::vector<int>> KMeans::get_clusters() const
{
    return clusters;
}