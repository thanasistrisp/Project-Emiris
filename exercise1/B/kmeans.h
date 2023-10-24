#ifndef KMEANS_H
#define KMEANS_H

#include <vector>
#include <tuple>
#include <unordered_set>
#include <unordered_map>

#include "lp_metric.h"

typedef enum {CLASSIC, REVERSE_LSH, REVERSE_HYPERCUBE} update_method;

class KMeans
{
    private:
        std::vector<std::vector<double>> centroids;
        std::vector<std::unordered_set<int>> clusters;

        std::vector<int> point_to_cluster;

        std::unordered_map<int, int> point_2_cluster;

        const std::vector<std::vector<double>> &dataset;

        void kmeanspp();

        double min_dist_centroids() const;

        void assign_lloyds_reverse();

        std::tuple<int,int> assign_lloyds(int);
        std::tuple<int,int> assign_lsh(int);
        std::tuple<int,int> assign_hypercube(int);

        bool update();
        bool update(int, int);
        bool update(int, int, int);

        int number_of_hash_tables, k_lsh, max_points_checked, k_hypercube, probes;

    public:
        KMeans(const std::vector<std::vector<double>>& dataset);
        ~KMeans();

        void compute_clusters(int, update_method, const std::tuple<int,int,int,int,int> &config);

        std::vector<std::vector<double>> get_centroids() const;
    
        std::vector<std::vector<int>> get_clusters() const;

        static constexpr double (*distance)(const std::vector<double>&, const std::vector<double>&) = euclidean_distance;
        
        double silhouette(int i);

        int get_dataset_size() const { return dataset.size(); }
};

#endif /* KMEANS_H */