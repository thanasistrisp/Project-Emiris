#ifndef KMEANS_H
#define KMEANS_H

#include <vector>
#include <tuple>
#include <unordered_set>

#include "metrics.hpp"

enum update_method {CLASSIC = 0, LSH = 1, HYPERCUBE = 2}; 

class KMeans
{
    private:
        std::vector<std::vector<double>> centroids;
        std::vector<std::unordered_set<int>> clusters;

        std::vector<int> point_to_cluster;

        std::vector<std::vector<double>> &dataset;

        void kmeanspp();


        std::tuple<int,int> assign_lloyds(int);
        std::tuple<int,int> assign_lsh(int);
        std::tuple<int,int> assign_hypercube(int);

        bool update();
        bool update(int, int);

    public:
        KMeans(std::vector<std::vector<double>>& dataset);
        ~KMeans();

        void compute_clusters(int, update_method);

        std::vector<std::vector<double>> get_centroids() const;
    
        std::vector<std::vector<int>> get_clusters() const;

        static constexpr double (*distance)(const std::vector<double>&, const std::vector<double>&) = euclidean_distance;
        
        int silhouette(int i);
};

#endif /* KMEANS_H */