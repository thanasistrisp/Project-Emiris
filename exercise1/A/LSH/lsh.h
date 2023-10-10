#ifndef LSH_H
#define LSH_H

#include <vector>

#include "hash_table.h"

class LSH
{
    private:
        const int number_of_dimensions;     // Number of dimensions d.
        const int number_of_hash_functions; // Number of hash functions k for each hash function g_j, j = 0, ..., M.

        const int table_size;            // Hash table size.
        const int number_of_hash_tables; // Number of hash tables L.
        HashTable<std::vector<double>, int> **hash_tables; // Hash tables.
    
    public:
        LSH(int, int, int, int, int);
        ~LSH();

        void insert(std::vector<double>, int);
};

#endif /* LSH_H */