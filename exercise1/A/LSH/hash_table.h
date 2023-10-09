#ifndef T_HASH_TABLE_H
#define T_HASH_TABLE_H

#include <iostream>
#include <vector>

#include "list.h"
#include "hash_function.h"

#define M ((1ULL << 32) - 5) // Large prime number for fast hashing.

template <typename V> class HashBucket
{
    private:
        const unsigned int id; // Bucket id.
        List<V> elements;

    public:
        HashBucket(int);
        ~HashBucket();

        unsigned int get_id() const;

        void insert(const V*);
};

template <typename K, typename V> class HashTable
{
    private:
        const int table_size;
        List<HashBucket<V>> **buckets;

        const int number_of_hash_functions;
        std::vector<HashFunction*> hash_functions;
        std::vector<int> primary_factors;
        std::vector<int> secondary_factors;

        int primary_hash_function(const K&);
        unsigned int secondary_hash_function(const K&);

    public:
        HashTable(int, int, int, int);
        ~HashTable();

        int get_table_size() const;

        void insert(const K*, const V*);
};

// ---------- Functions for class HashBucket ---------- //

template <typename V> HashBucket<V>::HashBucket(int id)
: id(id)
{
    
}

template <typename V> HashBucket<V>::~HashBucket()
{

}

template <typename V> unsigned int HashBucket<V>::get_id() const
{
    return id;
}

template <typename V> void HashBucket<V>::insert(const V* element)
{
    elements.insert_first(element);
}

// ---------- Functions for class HashTable ---------- //

template <typename K, typename V> HashTable<K, V>::HashTable(int table_size, int number_of_dimensions, int number_of_hash_functions, int window)
: table_size(table_size), number_of_hash_functions(number_of_hash_functions)
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

    // Initialize random factors for secondary hash function
    // h(p) = \sum_{i = 1}^{k}(r'_i * h_i(p)) \mod M.
    for(int i = 0; i < number_of_hash_functions; i++){
        secondary_factors.push_back(rand());
    }

    buckets = new List<HashBucket<V>>*[table_size];
}

template <typename K, typename V> HashTable<K, V>::~HashTable()
{
    std::vector<HashFunction *>::const_iterator iter;
    for(iter = hash_functions.begin(); iter != hash_functions.end(); std::advance(iter, 1)){
        delete *iter;
    }

    if(buckets != NULL){
        for(int i = 0; i < table_size; i++){
            if(buckets[i] != NULL){
                delete buckets[i];
            }
        }
        delete[] buckets;
    }
}

template <typename K, typename V> int HashTable<K, V>::primary_hash_function(const K& p)
{
    // Use primary hash function
    // g(p) = ( \sum_{i = 1}^{k}(r_i * h_i(p)) \mod M ) \mod table_size.
    int r_i, h_i, sum = 0;
    for(int i = 0; i < number_of_hash_functions; i++){
        r_i = primary_factors.at(i);
        h_i = hash_functions.at(i)->hash(p);
        sum += r_i * h_i;
    }
    return (sum % M) % table_size;
}

template <typename K, typename V> unsigned int HashTable<K, V>::secondary_hash_function(const K& p)
{
    // Use secondary hash function
    // h(p) = \sum_{i = 1}^{k}(r'_i * h_i(p)) \mod M.
    int r_i_prime, h_i, sum = 0;
    for(int i = 0; i < number_of_hash_functions; i++){
        r_i_prime = secondary_factors.at(i);
        h_i = hash_functions.at(i)->hash(p);
        sum += r_i_prime * h_i;
    }
    return sum % M;
}

template <typename K, typename V> int HashTable<K, V>::get_table_size() const
{
    return table_size;
}

template <typename K, typename V> void HashTable<K, V>::insert(const K *key, const V *value)
{
    // if bucket is already created, use secondary index to choose bucket in chain
    // else
        // traverse nodes to find the one with the id
        // if found, insert data there
        // else create new bucket
    int bucket_index = primary_hash_function(*key);
    unsigned int bucket_id = secondary_hash_function(*key);

    HashBucket<V> *bucket;
    List<HashBucket<V>> *list = buckets[bucket_index];
    if(list == NULL){
        list = new List<HashBucket<V>>;
        bucket = new HashBucket<V>(bucket_id);
        bucket->insert(value);
        list->insert_first(bucket);
        return;
    }
    for(int index = 0; ; index++){
        bucket = list->get_data(index);
        if(bucket == NULL){
            bucket = new HashBucket<V>(bucket_id);
            list->insert_first(bucket);
            bucket->insert(value);
            break;
        }
        if(bucket->get_id() == bucket_id){
            bucket->insert(value);
            break;
        }
    }
}

#endif /* T_HASH_TABLE_H */