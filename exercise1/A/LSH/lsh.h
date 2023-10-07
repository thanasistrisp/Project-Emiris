#include <vector>
// #include <unordered_map>

class HashFunction
{
    private:
        int number_of_dimensions; // Number of dimensions d.
        int window;               // Window w for the entire algorithm.
        float t;                  // Shift t.
        std::vector<double> v; // d-dimensional vector with coordinates in N(0, 1).

    public:
        HashFunction(int, int);
        ~HashFunction();

        int hash(std::vector<double>&);
};

class HashFunctionFamily
{
    private:
        int number_of_hash_functions;               // Number of hash functions k.
        std::vector<HashFunction *> hash_functions; // Hash functions h_i.

    public:
        HashFunctionFamily(int, int, int);
        ~HashFunctionFamily();

        int hash(int, std::vector<double>&);
};

class LSH
{
    private:
        const int number_of_dimensions;     // Number of dimensions d.
        const int number_of_hash_functions; // Number of hash functions k for each hash function g_j, j = 0, ..., M.

        const int number_of_hash_tables; // Number of hash tables L.
        const int number_of_buckets;     // Number of buckets for each hash table M.

        HashFunctionFamily hash_function_family; // Hash function family H = {h_1, h_2, ..., h_k}.
        // std::vector<std::unordered_map<std::vector<double>, int>> hash_tables; // Hash tables.
        
        std::vector<std::vector<int>> factors; // Factors for hash functions g_j.
    
    public:
        LSH(int, int, int, int, int);
        ~LSH();

        int hash(int, std::vector<double>&);
        // void insert(std::vector<double>&);
};