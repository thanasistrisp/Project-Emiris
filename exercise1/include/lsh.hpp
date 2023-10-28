#pragma once

#include <vector>
#include <tuple>

#include "hash_table.hpp"

class LSH
{
    private:
        const int number_of_dimensions;     // Number of dimensions d.
        const int number_of_hash_functions; // Number of hash functions k for each hash function g_j, j = 0, ..., M.

        const int table_size;            // Hash table size.
        const int number_of_hash_tables; // Number of hash tables L.
        HashTable<std::vector<double>, int> **hash_tables; // Hash tables.

        const std::vector<std::vector<double>> &dataset;
    
        // Inserts the given data point with the given index to all L hash tables. 
        void insert(std::vector<double>, int);

    public:
        // Initializes an instance with the given number of hash functions,
        // number of hash tables, table size and window.
        // The last argument is the set of points the LSH algorithm will be applied to.
        LSH(int, int, int, int, const std::vector<std::vector<double>>&);
        ~LSH();

        // Returns the indices of the k-approximate nearest neighbours (ANN) of the given query q
        // and their distances to the query based on the given distance function.
        std::tuple<std::vector<int>, std::vector<double>> query(const std::vector<double>&, unsigned int k,
                                                                double (*distance)(const std::vector<double>&, const std::vector<double>&));

        // Returns the indices of the k-approximate nearest neighbours (ANN) of the given query q
        // and their distances to the query based on the given distance function.
        // All the neighbours returned lie within radius r.
        std::tuple<std::vector<int>, std::vector<double>> query_range(const std::vector<double>&, double r,
                                                                double (*distance)(const std::vector<double>&, const std::vector<double>&));
};