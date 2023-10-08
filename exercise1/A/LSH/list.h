#ifndef T_LIST_H
#define T_LIST_H

#include <ostream>
#include <cstdlib>
// cstdlib is used for NULL macro.

template <typename T> class ListNode
{
    private:
        const T* data;
        ListNode* next_node;

    public:
        ListNode(const T*);
        ~ListNode();

        const T* get_data() const;
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

    public:
        List();
        ~List();
        // Deletes list and nodes, but not their contents.

        void insert_first(const T*);
        void insert_last(const T*);

        const T* remove_first();
        const T* remove_last();

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

template <typename T> ListNode<T>::ListNode(const T *data)
: data(data), next_node(NULL)
{

}

template <typename T> ListNode<T>::~ListNode()
{
    
}

template <typename T> const T* ListNode<T>::get_data() const
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
: head(NULL), count(0)
{

}

template <class T> List<T>::~List()
{
    while(head != NULL){
        remove_first();
    }
}

template <typename T> void List<T>::insert_first(const T *data)
{
    ListNode<T> *new_first_node = new ListNode<T>(data);
    ListNode<T> *old_first_node = head;
    new_first_node->set_next_node(old_first_node);
    head = new_first_node;
    count++;
}

template <typename T> void List<T>::insert_last(const T *data)
{
    if(count == 0){
        insert_first(data);
        return;
    }
    ListNode<T> *new_last_node = new ListNode(data);
    ListNode<T> *node = head;
    ListNode<T> *previous_node;
    while(node != NULL){
        previous_node = node;
        node = node->get_next_node();
    }
    previous_node->set_next_node(new_last_node);
    count++;
}

template <typename T> const T* List<T>::remove_first()
{
    if(count == 0){
        return NULL;
    }
    ListNode<T> *to_be_removed = head;
    ListNode<T> *new_first_node = to_be_removed->get_next_node();
    head = new_first_node;
    void *data = to_be_removed->get_data();
    delete to_be_removed;
    count--;
    return data;
}

template <typename T> const T* List<T>::remove_last()
{
    if(count == 0){
        return NULL;
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

template <typename T> int List<T>::get_count() const
{
    return count;
}

#endif /* T_LIST_H */