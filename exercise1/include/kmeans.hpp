#pragma once

#include <vector>
#include <tuple>
#include <unordered_set>
#include <unordered_map>

#include "lp_metric.hpp"

typedef enum {CLASSIC, REVERSE_LSH, REVERSE_HYPERCUBE} update_method;

class KMeans
{
    private:
        // Initializes centroids with random points from dataset using KMeans++ algorithm.
        void kmeanspp();

        double min_dist_centroids() const;
        double max_dist_centroids() const;

        // Uses the Classic KMeans algoritm (Lloyd's algorithm) to assign
        // any unassigned points in Reverse Search clustering algorithms.
        void assign_lloyds_reverse();

        // Assigns the given point to the cluster with the nearest centroid
        // and returns both old and new cluster of the point (they may be the same).
        std::tuple<int,int> assign_lloyds(int);

        // Updates the centroids (if needed) and returns true if the centroids changed.
        bool update();
        bool update(int, int, int);
        
        // Cluster computation using the Classic KMeans algoritm (Lloyd's algorithm),
        // Reverse Search using LSH and Reverse Search using Hypercube respectively.
        void compute_clusters_lloyds();
        void compute_clusters_reverse_lsh();
        void compute_clusters_reverse_hypercube();

        int number_of_hash_tables, k_lsh, max_points_checked, k_hypercube, probes, window;

    protected:
        std::vector<std::vector<double>> centroids;

        // Array of sets of points, where clusters[i] is the set of points of the i-th cluster
        // unordered_set is used to for fast lookup and deletion in specific cluster.
        std::vector<std::unordered_set<int>> clusters;

        // Point_to_cluster[i] = j means that the i-th point of the dataset belongs to the j-th cluster (zero-indexed).
        // Needs to  be updated together with clusters.
        std::vector<int> point_to_cluster;

        std::unordered_map<int, int> point_2_cluster;

        const std::vector<std::vector<double>> &dataset;
    public:
        // Initializes an instance.
        // The argument is the dataset the clustering algorithms will be applied to.
        KMeans(const std::vector<std::vector<double>>& dataset);

        // Computes internally the clusters using the number of clusters, the given method and the the following tuple:
        /*
         * 1. number L of the LSH (number of hash tables)
         * 2. k of the LSH (number of hash functions)
         * 3. M of the Hypercube (number of candidates)
         * 4. k of the Hypercube (number of dimensions)
         * 5. number of probes of the Hypercube
         */
        void compute_clusters(int, update_method, const std::tuple<int,int,int,int,int,double> &config);

        // Returns the centroid coordinates.
        std::vector<std::vector<double>> get_centroids() const;

        // Returns the indices of the datapoints inside each cluster.
        std::vector<std::vector<int>> get_clusters() const;

        int get_dataset_size() const { return dataset.size(); }

        static constexpr double (*distance)(const std::vector<double>&, const std::vector<double>&) = euclidean_distance;
        
        // Returns the silhouette of the i-th point of the dataset.
        double silhouette(int i);
};