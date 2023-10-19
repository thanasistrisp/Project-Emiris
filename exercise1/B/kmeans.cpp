#include <vector>
#include <set>

using namespace std;

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

void KMeans::update() // MacQueen's update rule
{
    for(int i = 0; i < k; i++){ // For each cluster
        for(int j = 0; j < (int) centroids[i].size(); j++){
            centroids[i][j] = 0; // Reset centroid
        }
        for(int j = 0; j < (int) clusters[i].size(); j++){ // For each point in cluster
            for(int l = 0; l < (int) dataset[clusters[i][j]].size(); l++){
                centroids[i][l] += dataset[clusters[i][j]][l]; // Add point's coordinates to centroid
            }
        }
        for(int j = 0; j < (int) centroids[i].size(); j++){
            centroids[i][j] /= clusters[i].size(); // Divide by number of points in cluster (mean)
        }
    }
}

void KMeans::update(int k1, int k2) { // recalculate centroids only for the two affected clusters
    set<int> s;
    s.insert(k1);
    s.insert(k2);
    for (int i : s) {
        for (int j = 0; j < (int) centroids[i].size(); j++) {
            centroids[i][j] = 0;
        }
        for (int j = 0; j < (int) clusters[i].size(); j++) {
            for (int l = 0; l < (int) dataset[clusters[i][j]].size(); l++) {
                centroids[i][l] += dataset[clusters[i][j]][l];
            }
        }
        for (int j = 0; j < (int) centroids[i].size(); j++) {
            centroids[i][j] /= clusters[i].size();
        }
    }

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