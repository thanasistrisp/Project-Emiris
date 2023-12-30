#pragma once

#include <iostream>
#include <vector>
#include <tuple>

#include "list.hpp"
#include "hash_function.hpp"

template <typename V> class HashBucket
{
    private:
        const unsigned int id; // Bucket id.
        List<V> elements;

    public:
        // Initializes a hash bucket with the given id.
        HashBucket(int);
        ~HashBucket();

        // Returns the bucket's id.
        unsigned int get_id() const;

        // Inserts the given value to the bucket.
        void insert(V);

        // Returns the value stored in the bucket specified by index (0-based indexing).
        // Second argument shows the validity of the value returned,
        // i.e. whether the returned value is a legit instance of type V. 
        V get_data(int, bool&);
};

template <typename K, typename V> class HashTable
{
    private:
        const int table_size; // Number of bucket chains.
        List<HashBucket<V>*> **buckets; // Bucket chains.

        const int number_of_hash_functions;        // Number of hash functions k for each hash function g_j, j = 0, ..., M.
        std::vector<HashFunction*> hash_functions; // Hash functions h_i, i = 0, ..., k.
        std::vector<int> primary_factors;          // Integers multiplied with h_i to produce the amplified index function g.

        int recent_chain_index;     // Index of the most recently accessed bucket chain.
        int recent_bucket_index;    // Index of the most recently accessed bucket inside the bucket chain.
        int recent_element_index;   // Index of the most recently accessed element inside the bucket.
        bool finished_chain_search; // Indicates if we have gone through all buckets in the most recently accessed bucket chain.

        const static unsigned int M = ((1ULL << 32) - 5); // Large prime number for fast hashing.

    public:
        // Initializes a hash table with the given table size, number of dimensions of data points stored,
        // number of hash functions and window.
        HashTable(int, int, int, double);
        ~HashTable();

        // Returns the size of the hash table.
        int get_table_size() const;

        // Returns the hashed value of the given key using the amplified index function g.
        int primary_hash_function(K);

        // Returns the ID of the element with the given key.
        unsigned int secondary_hash_function(K);

        // Inserts the given value with the given key inside the hash table.
        void insert(K, V);
 
        // Returns a tuple containing the value and the ID of one element that lies in the same bucket chain
        // as the element with the given key.
        // The i-th call returns the value of the i-th element inside the same bucket chain.
        // Second argument shows the validity of the value returned,
        // i.e. whether the returned value is a legit instance of type V. 
        std::tuple<V, unsigned int> get_data(K, bool&);
};

// ---------- Functions for class HashBucket ---------- //

// Initializes a hash bucket with the given id.
template <typename V> HashBucket<V>::HashBucket(int id)
: id(id)
{
    
}

template <typename V> HashBucket<V>::~HashBucket()
{

}

// Returns the bucket's id.
template <typename V> unsigned int HashBucket<V>::get_id() const
{
    return id;
}

// Inserts the given value to the bucket.
template <typename V> void HashBucket<V>::insert(V element)
{
    elements.insert_first(element);
}

// Returns the value stored in the bucket specified by index (0-based indexing).
// Second argument shows the validity of the value returned,
// i.e. whether the returned value is a legit instance of type V. 
template <typename V> V HashBucket<V>::get_data(int index, bool &valid)
{
    return elements.get_data(index, valid);
}

// ---------- Functions for class HashTable ---------- //

// Initializes a hash table with the given table size, number of dimensions of data points stored,
// number of hash functions and window.
template <typename K, typename V> HashTable<K, V>::HashTable(int table_size, int number_of_dimensions, int number_of_hash_functions, double window)
: table_size(table_size), number_of_hash_functions(number_of_hash_functions),
  recent_chain_index(0), recent_bucket_index(0), recent_element_index(0), finished_chain_search(true)
{
    // Initialize h_i functions, i = 1, ..., k.
    HashFunction *h;
    for(int i = 0; i < number_of_hash_functions; i++){
        h = new HashFunction(number_of_dimensions, window);
        hash_functions.push_back(h);
    }

    // Initialize random factors for primary hash function
    // g(p) = ( \sum_{i = 1}^{k}(r_i * h_i(p)) \mod M ) \mod table_size.
    for(int i = 0; i < number_of_hash_functions; i++){
        primary_factors.push_back(rand());
    }

    buckets = new List<HashBucket<V>*>*[table_size];
    for(int i = 0; i < table_size; i++){
        buckets[i] = NULL;
    }
}

template <typename K, typename V> HashTable<K, V>::~HashTable()
{
    std::vector<HashFunction *>::const_iterator iter;
    for(iter = hash_functions.begin(); iter != hash_functions.end(); std::advance(iter, 1)){
        delete *iter;
    }

    HashBucket<V> *bucket;
    if(buckets != NULL){
        for(int i = 0; i < table_size; i++){
            if(buckets[i] != NULL){
                while((bucket = buckets[i]->remove_first()) != NULL){
                    delete bucket;
                }
                delete buckets[i];
            }
        }
        delete[] buckets;
    }
}

// Returns the hashed value of the given key using the amplified index function g.
template <typename K, typename V> int HashTable<K, V>::primary_hash_function(K p)
{
    // Use primary hash function
    // g(p) = ( \sum_{i = 1}^{k}(r_i * h_i(p)) \mod M ) \mod table_size =
    //      = h(p) \mod table_size.
    return secondary_hash_function(p) % table_size;
}

// Returns the ID of the element with the given key.
template <typename K, typename V> unsigned int HashTable<K, V>::secondary_hash_function(K p)
{
    // Use secondary hash function
    // h(p) = \sum_{i = 1}^{k}(r_i * h_i(p)) \mod M.
    // Apply the following property
    // (a + b) mod m = ((a mod m) + (b mod m)) mod m
    // to avoid overflow.
    int r_i, h_i;
    unsigned int sum = 0;
    for(int i = 0; i < number_of_hash_functions; i++){
        r_i = primary_factors.at(i);
        h_i = hash_functions.at(i)->hash(p);
        sum = ((sum % M) + ((r_i * h_i) % M)) % M;
    }
    return sum;
}

// Returns the size of the hash table.
template <typename K, typename V> int HashTable<K, V>::get_table_size() const
{
    return table_size;
}

// Inserts the given value with the given key inside the hash table.
template <typename K, typename V> void HashTable<K, V>::insert(K key, V value)
{
    int bucket_index = primary_hash_function(key);
    unsigned int bucket_id = secondary_hash_function(key);
    bool valid, inserted = false;
    HashBucket<V> *bucket;
    List<HashBucket<V>*> *list = buckets[bucket_index];
    if(list == NULL){ // No buckets in chain, add new bucket to store value.
        list = new List<HashBucket<V>*>;
        bucket = new HashBucket<V>(bucket_id);
        bucket->insert(value);
        list->insert_first(bucket);
        buckets[bucket_index] = list;
        return;
    }

    // Traverse the bucket chain and use secondary index to find the right bucket.
    for(int index = 0; index < list->get_count(); index++){
        bucket = list->get_data(index, valid);
        if(bucket->get_id() == bucket_id){
            bucket->insert(value);
            inserted = true;
            break;
        }
    }
    
    // If such bucket is not found, add new bucket to the chain and store value.
    if(!inserted){
        bucket = new HashBucket<V>(bucket_id);
        bucket->insert(value);
        list->insert_last(bucket);
    }
}

// Returns a tuple containing the value and the ID of one element that lies in the same bucket chain
// as the element with the given key.
// The i-th call returns the value of the i-th element inside the same bucket chain.
// Second argument shows the validity of the value returned,
// i.e. whether the returned value is a legit instance of type V.
template <typename K, typename V> std::tuple<V, unsigned int> HashTable<K, V>::get_data(K key, bool &valid)
{
    List<HashBucket<V>*> *chain;
    HashBucket<V> *bucket;
    V element;
    unsigned int bucket_id = -1;
    bool fvalid = false; // To be used in this scope only.
    valid = false;

    if(finished_chain_search){
        recent_chain_index = primary_hash_function(key);
        recent_bucket_index = 0;
        recent_element_index = 0;
        finished_chain_search = false;
    }
    chain = buckets[recent_chain_index];

    // Check again, in case recent_chain_index is obsolete.
    if(chain == NULL){
        finished_chain_search = true;
        return std::make_tuple(V(), bucket_id);
    }

    // If the chain exists, check if we have seen through all buckets.
    bucket = chain->get_data(recent_bucket_index, fvalid);
    if(bucket == NULL){
        finished_chain_search = true;
        return std::make_tuple(V(), bucket_id);
    }
    recent_element_index++;
    element = bucket->get_data(recent_element_index, valid);
    bucket_id = bucket->get_id();

    // If reached the end of the bucket, look for the next bucket in the same chain.
    if(element == V() && valid == false){
        recent_bucket_index++;
        bucket = chain->get_data(recent_bucket_index, fvalid);
        if(bucket == NULL){
            finished_chain_search = true;
            return std::make_tuple(V(), bucket_id);
        }
        recent_element_index = 0;
        element = bucket->get_data(recent_element_index, valid);
        bucket_id = bucket->get_id();
        if(element == V() && valid == false){
            finished_chain_search = true;
            return std::make_tuple(element, bucket_id);
        }
    }
    valid = true;
    return std::make_tuple(element, bucket_id);
}