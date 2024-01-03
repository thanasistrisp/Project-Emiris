#include <iostream>
#include <tuple>
#include <variant>

#include "kmeans_new.hpp"
#include "brute_force.hpp"

using namespace std;

KMeansNew::KMeansNew(const std::vector<std::vector<double>>& dataset) : KMeans(dataset), dataset_size(dataset.size()) {}

void KMeansNew::compute_decoded(vector<vector<double>> &decoded_dataset, vector<vector<double>> &initial_dataset) {
    // project centroids to initial space (decoded_centroids)
	decoded_centroids = new vector<vector<double>>(centroids.size());
	for (int i = 0; i < (int) centroids.size(); i++) {
		int enc_index = get<0>(brute_force(dataset, centroids[i], 1))[0];
		(*decoded_centroids)[i] = initial_dataset[get<0>(brute_force(initial_dataset, decoded_dataset[enc_index], 1))[0]];
	}

	// project points to initial space (decoded_points)
	decoded_points = new vector<vector<double>>(dataset.size());
	for (int i = 0; i < (int) dataset.size(); i++) {
		(*decoded_points)[i] = initial_dataset[get<0>(brute_force(initial_dataset, decoded_dataset[i], 1))[0]];
        cout << "Decoded point " << i << endl;
	}
}

vector<variant<double, vector<double>>> KMeansNew::silhouette() {
	// compute silhouette
	vector<double> si(clusters.size(), 0);
	vector<double> sil(clusters.size(), 0);
    vector<vector<int>> clusters = get_clusters();
    double stotal = 0;
    for (int i = 0; i < (int) clusters.size(); i++) {
        for (int j = 0; j < (int) clusters[i].size(); j++) {
            si[i] += silhouette(clusters[i][j]);
        }   
        stotal += si[i];
        si[i] /= clusters[i].size();
        sil[i] = si[i];
    }
    stotal /= dataset_size;

	// free memory
	delete decoded_centroids;
	delete decoded_points;	

    // return stotal, sil
    return {stotal, sil};
}

double KMeansNew::silhouette(int i)
{
    int cluster = point_to_cluster[i];
    double a = 0, b = 0;
    for(int j : clusters[cluster]){
        a += distance((*decoded_points)[i], (*decoded_points)[j]);
    }
    a /= clusters[cluster].size();

    // Find second closest cluster.
    int c1 = cluster, c2 = -1;
    for(int j = 0; j < (int) centroids.size(); j++){
        if(j != c1){
            double dist = distance((*decoded_points)[i], (*decoded_centroids)[j]);
            if(c2 == -1 || dist < distance((*decoded_points)[i], (*decoded_centroids)[c2])){
                c2 = j;
            }
        }
    }
    for(int j : clusters[c2]){
        b += distance((*decoded_points)[i], (*decoded_points)[j]);
    }
    b /= clusters[c2].size();
    
    return (b - a) / max(a, b);
}