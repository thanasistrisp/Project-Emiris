#ifndef LSH_H
#define LSH_H

#include <vector>
#include <tuple>

#include "hash_table.h"

class LSH
{
    private:
        const int number_of_dimensions;     // Number of dimensions d.
        const int number_of_hash_functions; // Number of hash functions k for each hash function g_j, j = 0, ..., M.

        const int table_size;            // Hash table size.
        const int number_of_hash_tables; // Number of hash tables L.
        HashTable<std::vector<double>, int> **hash_tables; // Hash tables.

        const std::vector<std::vector<double>> &dataset;
    
        void insert(std::vector<double>, int);

    public:
        LSH(int, int, int, const std::vector<std::vector<double>>&);
        ~LSH();

        std::tuple<std::vector<int>, std::vector<double>> query(const std::vector<double>&, unsigned int k,
                                                                double (*distance)(const std::vector<double>&, const std::vector<double>&));

        std::tuple<std::vector<int>, std::vector<double>> query_range(const std::vector<double>&, double r,
                                                                double (*distance)(const std::vector<double>&, const std::vector<double>&),
                                                                int min_bucket_elements = -1);
};

#endif /* LSH_H */