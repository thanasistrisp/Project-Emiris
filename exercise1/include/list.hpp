#pragma once

#include <ostream>
#include <cstdlib>
// cstdlib is used for NULL macro.

template <typename T> class ListNode
{
    private:
        T data;
        ListNode* next_node;

    public:
        ListNode(T);
        ~ListNode();

        T get_data() const;
        ListNode* get_next_node() const;

        void set_next_node(ListNode *);

        friend std::ostream& operator<<(std::ostream& os, const ListNode<T>& node)
        {
            return os << *(node.data);
        }
};

template <typename T> class List
{
    private:
        ListNode<T> *head;
        int count;

        ListNode<T> *recent_node;
        int recent_index;

    public:
        List();
        ~List();
        // Deletes list and nodes, but not their contents.

        void insert_first(T);
        void insert_last(T);

        T remove_first();
        T remove_last();

        T get_data(int, bool&);

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

template <typename T> ListNode<T>::ListNode(T data)
: data(data), next_node(NULL)
{

}

template <typename T> ListNode<T>::~ListNode()
{
    
}

template <typename T> T ListNode<T>::get_data() const
{
    return data;
}

template <typename T> ListNode<T>* ListNode<T>::get_next_node() const
{
    return next_node;
}

template <typename T> void ListNode<T>::set_next_node(ListNode<T> *node)
{
    next_node = node;
}

// ---------- Functions for class List ---------- //

template <class T> List<T>::List()
: head(NULL), count(0), recent_node(head), recent_index(0)
{

}

template <class T> List<T>::~List()
{
    while(head != NULL){
        remove_first();
    }
}

template <typename T> void List<T>::insert_first(T data)
{
    ListNode<T> *new_first_node = new ListNode<T>(data);
    ListNode<T> *old_first_node = head;
    new_first_node->set_next_node(old_first_node);
    head = new_first_node;
    count++;
}

template <typename T> void List<T>::insert_last(T data)
{
    if(count == 0){
        insert_first(data);
        return;
    }
    ListNode<T> *new_last_node = new ListNode<T>(data);
    ListNode<T> *node = head;
    ListNode<T> *previous_node;
    while(node != NULL){
        previous_node = node;
        node = node->get_next_node();
    }
    previous_node->set_next_node(new_last_node);
    count++;
}

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

// valid is true if the value is valid (i.e. legit instance of type T stored in the list)
template <typename T> T List<T>::get_data(int index, bool &valid)
{
    // if recent node = null, search from the start (index 0) until index
    // if recent index is the right previous index, then go to next node and return
    // if recent index < index, start from recent index and traverse until you find the index
    // if index < recent index, start from the begining
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

template <typename T> int List<T>::get_count() const
{
    return count;
}