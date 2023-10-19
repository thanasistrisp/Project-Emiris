#include <vector>

#include "kmeans.h"

KMeans::KMeans(std::vector<std::vector<double>> &dataset)
: dataset(dataset)
{
    
}

KMeans::~KMeans()
{

}

void KMeans::kmeanspp()
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

void KMeans::compute_clusters(int k, update_method method, std::vector<int> method_args,
                              double (*distance)(const std::vector<double>&, const std::vector<double>&))
{
    // Set assigning function depending on method.
    void (KMeans::*assign)(int);
    if(method == CLASSIC){
        assign = assign_lloyds;
    }
    else if(method == LSH){
        assign = assign_lsh;
    }
    else{
        assign = assign_hypercube;
    }

    // Initialize centroids using KMeans++ algorithm.
    kmeanspp();
    while(true){
        for(int i = 0; i < dataset.size(); i++){
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