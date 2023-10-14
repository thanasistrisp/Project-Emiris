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

        void insert(V);

        V get_data(int, bool&);
};

template <typename K, typename V> class HashTable
{
    private:
        const int table_size;
        List<HashBucket<V>*> **buckets;

        const int number_of_hash_functions;
        std::vector<HashFunction*> hash_functions;
        std::vector<int> primary_factors;

        int recent_chain_index;
        int recent_bucket_index;
        int recent_element_index;
        bool finished_chain_search;

    public:
        HashTable(int, int, int, int);
        ~HashTable();

        int get_table_size() const;

        int primary_hash_function(K);
        unsigned int secondary_hash_function(K);

        void insert(K, V);

        V get_data(K, bool&);
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

template <typename V> void HashBucket<V>::insert(V element)
{
    elements.insert_first(element);
}

template <typename V> V HashBucket<V>::get_data(int index, bool &valid)
{
    return elements.get_data(index, valid);
}

// ---------- Functions for class HashTable ---------- //

template <typename K, typename V> HashTable<K, V>::HashTable(int table_size, int number_of_dimensions, int number_of_hash_functions, int window)
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

template <typename K, typename V> int HashTable<K, V>::primary_hash_function(K p)
{
    // Use primary hash function
    // g(p) = ( \sum_{i = 1}^{k}(r_i * h_i(p)) \mod M ) \mod table_size =
    //      = h(p) \mod table_size.
    return secondary_hash_function(p) % table_size;
}

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

template <typename K, typename V> int HashTable<K, V>::get_table_size() const
{
    return table_size;
}

template <typename K, typename V> void HashTable<K, V>::insert(K key, V value)
{
    // if bucket is already created, use secondary index to choose bucket in chain
    // else
        // traverse nodes to find the one with the id
        // if found, insert data there
        // else create new bucket
    int bucket_index = primary_hash_function(key);
    unsigned int bucket_id = secondary_hash_function(key);
    bool valid, inserted = false;
    HashBucket<V> *bucket;
    List<HashBucket<V>*> *list = buckets[bucket_index];
    if(list == NULL){
        list = new List<HashBucket<V>*>;
        bucket = new HashBucket<V>(bucket_id);
        bucket->insert(value);
        list->insert_first(bucket);
        buckets[bucket_index] = list;
        return;
    }
    for(int index = 0; index < list->get_count(); index++){
        bucket = list->get_data(index, valid);
        if(bucket->get_id() == bucket_id){
            bucket->insert(value);
            inserted = true;
            break;
        }
    }
    if(!inserted){
        bucket = new HashBucket<V>(bucket_id);
        bucket->insert(value);
        list->insert_last(bucket);
    }
}

template <typename K, typename V> V HashTable<K, V>::get_data(K key, bool &valid)
{
    List<HashBucket<V>*> *chain; //= buckets[recent_chain_index];
    HashBucket<V> *bucket;
    V element;
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
        return V();
    }

    // If the chain exists, check if we have seen through all buckets.
    bucket = chain->get_data(recent_bucket_index, fvalid);
    if(bucket == NULL){
        finished_chain_search = true;
        return V();
    }
    recent_element_index++;
    element = bucket->get_data(recent_element_index, valid);

    // If reached the end of the bucket, look for the next bucket in the same chain.
    if(element == V() && valid == false){
        recent_bucket_index++;
        bucket = chain->get_data(recent_bucket_index, fvalid);
        if(bucket == NULL){
            finished_chain_search = true;
            return V();
        }
        recent_element_index = 0;
        element = bucket->get_data(recent_element_index, valid);
        if(element == V() && valid == false){
            finished_chain_search = true;
            return element;
        }
    }
    valid = true;
    return element;
}

#endif /* T_HASH_TABLE_H */