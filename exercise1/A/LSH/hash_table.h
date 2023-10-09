#ifndef T_HASH_TABLE_H
#define T_HASH_TABLE_H

#include <iostream>

#include "list.h"

template <typename T> class HashBucket
{
    private:
        const unsigned int id; // Bucket id.
        List<T> elements;

    public:
        HashBucket(int);
        ~HashBucket();

        unsigned int get_id() const;

        void insert(const T*);
};

template <typename T> class HashTable
{
    private:
        const int table_size;
        List<HashBucket<T>> **buckets;
        int (*primary_hash_function)(const T&);
        unsigned int (*secondary_hash_function)(const T&);

    public:
        HashTable(int, int (*)(const T&), unsigned int (*)(const T&));
        ~HashTable();

        void insert(const T*);
};

// ---------- Functions for class HashBucket ---------- //

template <typename T> HashBucket<T>::HashBucket(int id)
: id(id)
{
    
}

template <typename T> HashBucket<T>::~HashBucket()
{

}

template <typename T> unsigned int HashBucket<T>::get_id() const
{
    return id;
}

template <typename T> void HashBucket<T>::insert(const T* element)
{
    elements.insert_first(element);
}

// ---------- Functions for class HashTable ---------- //

template <typename T> HashTable<T>::HashTable(int table_size, int (*primary)(const T&),
                                              unsigned int (*secondary)(const T&))
: table_size(table_size), primary_hash_function(primary),
  secondary_hash_function(secondary)
{
    buckets = new List<HashBucket<T>>*[table_size];
}

template <typename T> HashTable<T>::~HashTable()
{
    for(int i = 0; i < table_size; i++){
        if(buckets[i] != NULL){
            delete buckets[i];
        }
    }
    delete[] buckets;
}

template <typename T> void HashTable<T>::insert(const T *data)
{
    // if bucket is already created, use secondary index to choose bucket in chain
    // else
        // traverse nodes to find the one with the id
        // if found, insert data there
        // else create new bucket
    int bucket_index = primary_hash_function(*data);
    unsigned int bucket_id = secondary_hash_function(*data);

    HashBucket<T> *bucket;
    List<HashBucket<T>> *list = buckets[bucket_index];
    if(list == NULL){
        list = new List<HashBucket<T>>;
        bucket = new HashBucket<T>(bucket_id);
        bucket->insert(data);
        list->insert_first(bucket);
        return;
    }
    for(int index = 0; ; index++){
        bucket = list->get_data(index);
        if(bucket == NULL){
            bucket = new HashBucket<T>(bucket_id);
            list->insert_first(bucket);
            bucket->insert(data);
            break;
        }
        if(bucket->get_id() == bucket_id){
            bucket->insert(data);
            break;
        }
    }
}

#endif /* T_HASH_TABLE_H */