#include <iostream>
#include <vector>
#include <set>
#include <tuple>
#include <unordered_map>

using namespace std;

#include "kmeans.h"
#include "defines.hpp"

#include "lsh.h"
#include "hypercube.hpp"

KMeans::KMeans(const vector<std::vector<double>> &dataset) : dataset(dataset)
{
    point_to_cluster.resize(dataset.size());
}

KMeans::~KMeans()
{

}

tuple<int,int> KMeans::assign_lloyds(int index)
{
    int old_cluster = point_to_cluster[index];
    int new_cluster = 0;
    bool changed = false;
    double min_dist = distance(dataset[index], centroids[0]);
    for(int i = 1; i < (int) centroids.size(); i++){
        double dist = distance(dataset[index], centroids[i]);
        if(dist < min_dist){
            min_dist = dist;
            new_cluster = i;
            changed = true;
        }
    }
    if (changed) {
        point_to_cluster[index] = new_cluster; // update point's cluster
        clusters[old_cluster].erase(index); // remove point from old cluster
        clusters[new_cluster].insert(index); // add point to new cluster
    }
    return make_tuple(old_cluster, new_cluster);
}

tuple<int,int> KMeans::assign_lsh(int index)
{
    index++; // 

    // Index n points into L hashtables: once for the entire algorithm.
    static LSH lsh(k_lsh, number_of_hash_tables, w, dataset);

    double dist, radius = distance(centroids[0], centroids[1]);
    for(int i = 0; i < (int) centroids.size(); i++){
        for(int j = i + 1; j < (int) centroids.size(); j++){
            dist = distance(centroids[i], centroids[j]);
            if(dist < radius){
                radius = dist;
            }
        }
    }

    // Start with radius = min(dist between centroids) / 2.
    radius /= 2;
    bool changed_assignment = true;
    vector<int> ball;
    vector<double> distances;
    int p_index;
    unordered_map<int, int>::const_iterator iter;
    while(changed_assignment){
        changed_assignment = false;
        for(int i = 0; i < (int) centroids.size(); i++){
            // At each iteration, for each centroid c, range/ball queries centered at c.
            // Avoid buckets with very few items.
            tie(ball, distances) = lsh.query_range(centroids[i], radius, distance, dataset.size() >> 3);
            for(int j = 0; j < (int) ball.size(); j++){
                p_index = ball[j];
                iter = point_2_cluster.find(p_index);
                if(iter == point_2_cluster.end()){
                    point_2_cluster[p_index] = i;
                    clusters[i].insert(p_index);
                    changed_assignment = true;
                }
                // If the point lies in >= 2 balls, assign it to the cluster with the closest centroid.
                else if(distances[j] < distance(centroids[iter->second], dataset[p_index])){
                    clusters[iter->second].erase(p_index);
                    point_2_cluster[p_index] = i;
                    clusters[i].insert(p_index);
                    changed_assignment = true;
                }
            }
        }
        radius *= 2; // Multiply radius by 2.
    }

    // For every unassigned point, compare its distances to all centers
    // i.e. apply Lloyd's method for assignment.
    int old_cluster, new_cluster;
    for(int i = 0; i < (int) dataset.size(); i++){
        iter = point_2_cluster.find(i);
        if(iter == point_2_cluster.end()){
            clusters[0].insert(i);
            point_to_cluster[i] = 0;
            point_2_cluster[i] = 0;
        }
        tie(old_cluster, new_cluster) = assign_lloyds(i);
        point_to_cluster[i] = new_cluster;
        point_2_cluster[p_index] = new_cluster;
    }
    return make_tuple(-1,-1);
}

tuple<int,int> KMeans::assign_hypercube(int index)
{
    static hypercube hypercube(dataset, k_hypercube, max_points_checked, probes);
    cout << index << endl;
    return make_tuple(-1,-1);
}

bool KMeans::update() // MacQueen's update rule
{
    bool changed_centroids = false;
    for(int i = 0; i < (int) centroids.size(); i++){ // for each cluster
        vector<double> new_centroid(dataset[0].size(), 0);
        for(int j : clusters[i]){ // for each point in cluster
            for(int l = 0; l < (int) dataset[j].size(); l++){
                new_centroid[l] += dataset[j][l]; // add point's coordinates
            }
        }
        for(int l = 0; l < (int) new_centroid.size(); l++){
            new_centroid[l] /= clusters[i].size(); // divide by number of points
        }
        if(new_centroid != centroids[i]){
            centroids[i] = new_centroid;
            changed_centroids = true;
        }
    }
    return changed_centroids;
}

bool KMeans::update(int old_cluster, int new_cluster)
{
    bool changed_centroids = false;
    vector<double> new_centroid(dataset[0].size(), 0);
    for(int j : clusters[old_cluster]){
        for(int l = 0; l < (int) dataset[j].size(); l++){
            new_centroid[l] += dataset[j][l];
        }
    }
    for(int l = 0; l < (int) new_centroid.size(); l++){
        new_centroid[l] /= clusters[old_cluster].size();
    }
    if(new_centroid != centroids[old_cluster]){
        centroids[old_cluster] = new_centroid;
        changed_centroids = true;
    }
    new_centroid = vector<double>(dataset[0].size(), 0);
    for(int j : clusters[new_cluster]){
        for(int l = 0; l < (int) dataset[j].size(); l++){
            new_centroid[l] += dataset[j][l];
        }
    }
    for(int l = 0; l < (int) new_centroid.size(); l++){
        new_centroid[l] /= clusters[new_cluster].size();
    }
    if(new_centroid != centroids[new_cluster]){
        centroids[new_cluster] = new_centroid;
        changed_centroids = true;
    }
    return changed_centroids;
}

void KMeans::compute_clusters(int k, update_method method, const tuple<int,int,int,int, int> &config) {
    tie(number_of_hash_tables, k_lsh, max_points_checked, k_hypercube, probes) = config;
    clusters.resize(k);
    
    tuple<int,int> (KMeans::*assign)(int);
    if(method == CLASSIC){
        assign = &KMeans::assign_lloyds;
        // add all points to cluster 0
        for(int i = 0; i < (int) dataset.size(); i++){
            clusters[0].insert(i);
            point_to_cluster[i] = 0;
        }
    }
    else if(method == REVERSE_LSH){
        assign = &KMeans::assign_lsh;
    }
    else if(method == REVERSE_HYPERCUBE){
        assign = &KMeans::assign_hypercube;
    }

    bool changed_centroids;

    // Initialize centroids using KMeans++ algorithm.
    kmeanspp();
    bool first = true; // flag to avoid updating centroids on first iteration
    while(true){
        if(method == CLASSIC){
            for(int i = 0; i < (int) dataset.size(); i++){
                // Assign point to cluster and apply MacQueen's update rule.
                int old_cluster, new_cluster;
                tie(old_cluster, new_cluster) = (this->*assign)(i);
                if(method == CLASSIC && old_cluster != new_cluster && !first){
                    update(old_cluster, new_cluster);
                }
            }
            first = false;
        }
        else{
            (this->*assign)(0);
        }
        changed_centroids = update();
        if(!changed_centroids){
            break;
        }
    }
}

std::vector<std::vector<double>> KMeans::get_centroids() const
{
    return centroids;
}

std::vector<std::vector<int>> KMeans::get_clusters() const
{
    vector<vector<int>> clusters_vector;
    for(int i = 0; i < (int) clusters.size(); i++){
        vector<int> cluster;
        for(int j : clusters[i]){
            cluster.push_back(j);
        }
        clusters_vector.push_back(cluster);
    }
    return clusters_vector;
}

double KMeans::silhouette(int i)
{
    int cluster = point_to_cluster[i];
    double a = 0, b = 0;
    for(int j : clusters[cluster]){
        a += distance(dataset[i], dataset[j]);
    }
    a /= clusters[cluster].size();

    int c1 = cluster, c2 = -1;
    for(int j = 0; j < (int) centroids.size(); j++){
        if(j != c1){
            double dist = distance(dataset[i], centroids[j]);
            if(c2 == -1 || dist < distance(dataset[i], centroids[c2])){
                c2 = j;
            }
        }
    }
    for(int j : clusters[c2]){
        b += distance(dataset[i], dataset[j]);
    }
    b /= clusters[c2].size();
    
    return (b - a) / max(a, b);
}