#ifndef KMEANS_H
#define KMEANS_H

#include <vector>

enum update_method {CLASSIC = 0, LSH = 1, HYPERCUBE = 2}; 

class KMeans
{
    private:
        std::vector<double> centroids;
        std::vector<std::vector<int>> clusters;

        std::vector<std::vector<double>> &dataset;

        void kmeanspp();

        void assign();

        void update();

    public:
        KMeans(std::vector<std::vector<double>>&);
        ~KMeans();

        void compute_clusters(int, update_method, std::vector<int>,
                              double (*distance)(const std::vector<double>&, const std::vector<double>&));

        std::vector<double> get_centroids() const;
    
        std::vector<std::vector<int>> get_clusters() const;
};

#endif /* KMEANS_H */