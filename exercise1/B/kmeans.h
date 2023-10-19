#ifndef KMEANS_H
#define KMEANS_H

#include <vector>

#include "metrics.hpp"

enum update_method {CLASSIC = 0, LSH = 1, HYPERCUBE = 2}; 

class KMeans
{
    private:
        std::vector<std::vector<double>> centroids;
        std::vector<std::vector<int>> clusters;

        std::vector<std::vector<double>> &dataset;
        int k;

        void kmeanspp();

        void assign_lloyds(int);
        void assign_lsh(int);
        void assign_hypercube(int);

        void update();
        void update(int, int);

    public:
        KMeans(std::vector<std::vector<double>>& dataset, int k);
        ~KMeans();

        void compute_clusters(int, update_method, std::vector<int>, const std::vector<double>&);

        std::vector<std::vector<double>> get_centroids() const;
    
        std::vector<std::vector<int>> get_clusters() const;

        static constexpr double (*distance)(const std::vector<double>&, const std::vector<double>&) = euclidean_distance;
};

#endif /* KMEANS_H */