#pragma once

#include <ostream>
#include <cstdlib>
// cstdlib is used for NULL macro.

// Template class ListNode, representing the nodes of the list.
template <typename T> class ListNode
{
    private:
        T data;
        ListNode* next_node;

    public:
        // Initializes a list node with the given data.
        ListNode(T);
        ~ListNode();

        // Returns the data stored in the node.
        T get_data() const;

        // Returns a pointer to the next node.
        ListNode* get_next_node() const;

        // Sets the given node as the next node.
        void set_next_node(ListNode *);

        friend std::ostream& operator<<(std::ostream& os, const ListNode<T>& node)
        {
            return os << *(node.data);
        }
};

// Template class List.
template <typename T> class List
{
    private:
        ListNode<T> *head;
        int count;

        ListNode<T> *recent_node; // Most recently accessed node, meant to be used as an iterator.
        int recent_index;         // Index of the most recently accessed node.

    public:
        List();

        // Deletes list and nodes, but not their contents.
        ~List();

        // Inserts the given data in the first node.
        void insert_first(T);

        // Inserts the given data in the last node.
        void insert_last(T);

        // Removes and returns the data stored in the first node.
        T remove_first();

        // Removes and returns the data stored in the last node.
        T remove_last();

        // Returns the data stored in the node specified by index (0-based indexing).
        // Second argument shows the validity of the value returned,
        // i.e. whether the returned value is a legit instance of type T.
        T get_data(int, bool&);

        // Returns the number of nodes in the list.
        int get_count() const;

        friend std::ostream& operator<<(std::ostream& os, const List<T>& list)
        {
            ListNode<T> *current;
            current = list.head;
            while(current != NULL){
                os << (*current) << std::endl;
                current = current->get_next_node();
            }
            return os;
        }
};

// ---------- Functions for class ListNode ---------- //

// Initializes a list node with the given data.
template <typename T> ListNode<T>::ListNode(T data)
: data(data), next_node(NULL)
{

}

template <typename T> ListNode<T>::~ListNode()
{
    
}

// Returns the data stored in the node.
template <typename T> T ListNode<T>::get_data() const
{
    return data;
}

// Returns a pointer to the next node.
template <typename T> ListNode<T>* ListNode<T>::get_next_node() const
{
    return next_node;
}

// Sets the given node as the next node.
template <typename T> void ListNode<T>::set_next_node(ListNode<T> *node)
{
    next_node = node;
}

// ---------- Functions for class List ---------- //

template <class T> List<T>::List()
: head(NULL), count(0), recent_node(head), recent_index(0)
{

}

// Deletes list and nodes, but not their contents.
template <class T> List<T>::~List()
{
    while(head != NULL){
        remove_first();
    }
}

// Inserts the given data in the first node.
template <typename T> void List<T>::insert_first(T data)
{
    ListNode<T> *new_first_node = new ListNode<T>(data);
    ListNode<T> *old_first_node = head;
    new_first_node->set_next_node(old_first_node);
    head = new_first_node;
    count++;
}

// Inserts the given data in the last node.
template <typename T> void List<T>::insert_last(T data)
{
    if(count == 0){
        insert_first(data);
        return;
    }
    ListNode<T> *new_last_node = new ListNode<T>(data);
    ListNode<T> *node = head;
    ListNode<T> *previous_node = head;
    while(node != NULL){
        previous_node = node;
        node = node->get_next_node();
    }
    previous_node->set_next_node(new_last_node);
    count++;
}

// Removes and returns the data stored in the first node.
template <typename T> T List<T>::remove_first()
{
    if(count == 0){
        return T();
    }
    ListNode<T> *to_be_removed = head;
    ListNode<T> *new_first_node = to_be_removed->get_next_node();
    head = new_first_node;
    T data = to_be_removed->get_data();
    delete to_be_removed;
    count--;
    return data;
}

// Removes and returns the data stored in the last node.
template <typename T> T List<T>::remove_last()
{
    if(count == 0){
        return T();
    }
    if(head->get_next_node() == NULL){
        return remove_first();
    }
    ListNode<T> *node = head;
    ListNode<T> *previous_node;
    ListNode<T> *to_be_removed;
    while(node->get_next_node() != NULL){
        previous_node = node;
        node = node->get_next_node();
    }
    to_be_removed = node;
    previous_node->set_next_node(NULL);
    void *data = to_be_removed->get_data();
    delete to_be_removed;
    count--;
    return data;
}

// Returns the data stored in the node specified by index (0-based indexing).
// Second argument shows the validity of the value returned,
// i.e. whether the returned value is a legit instance of type T.
template <typename T> T List<T>::get_data(int index, bool &valid)
{
    // If index is out of bounds, return V().
    // If recent node = NULL, start from the beginning (index 0) until index is found.
    // If index < recent index, start from the begining (index 0) until index is found.
    // If recent index is the right previous index, then move recent node to the next node and return its data.
    valid = false;

    // Check if index is out of bounds.
    if(index < 0 || index >= count){
        return T();
    }
    
    if(recent_node == NULL){
        recent_node = head;
        recent_index = 0;
    }
    if(index < recent_index){
        recent_node = head;
        for(recent_index = 0; recent_node != NULL && recent_index < index; recent_index++){
            recent_node = recent_node->get_next_node();
            recent_index++;
        }
        if(recent_node != NULL){
            valid = true;
            return recent_node->get_data();
        }
        return T();
    }
    if(index == recent_index + 1){
        recent_node = recent_node->get_next_node();
        recent_index++;
        valid = true;
        return recent_node->get_data();
    }
    valid = true;
    return recent_node->get_data();
}

// Returns the number of nodes in the list.
template <typename T> int List<T>::get_count() const
{
    return count;
}