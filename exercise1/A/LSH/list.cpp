#include <cstdlib>
// cstdlib is used for NULL macro.

#include "list.h"

// ---------- Functions of class ListNode ---------- //

ListNode::ListNode(void *element)
: element(element), next_node(NULL)
{

}

ListNode::~ListNode()
{

}

void* ListNode::get_element()
{
    return element;
}

ListNode* ListNode::get_next_node()
{
    return next_node;
}

void ListNode::set_next_node(ListNode *next_node)
{
    this->next_node = next_node;
}

// ---------- Functions of class List ---------- //

List::List()
: head(NULL), count(0)
{

}

List::~List()
{
    while(head != NULL){
        remove_first();
    }
}

void List::insert_first(void *element)
{
    ListNode *new_first_node = new ListNode(element);
    ListNode *old_first_node = head;
    new_first_node->set_next_node(old_first_node);
    head = new_first_node;
    count++;
}

void List::insert_last(void *element)
{
    if(count == 0){
        insert_first(element);
        return;
    }
    ListNode *new_last_node = new ListNode(element);
    ListNode *node = head;
    ListNode *previous_node;
    while(node != NULL){
        previous_node = node;
        node = node->get_next_node();
    }
    previous_node->set_next_node(new_last_node);
    count++;
}

void* List::remove_first()
{
    if(count == 0){
        return NULL;
    }
    ListNode *to_be_removed = head;
    ListNode *new_first_node = to_be_removed->get_next_node();
    head = new_first_node;
    void *element = to_be_removed->get_element();
    delete to_be_removed;
    count--;
    return element;
}

void* List::remove_last()
{
    if(count == 0){
        return NULL;
    }
    if(head->get_next_node() == NULL){
        return remove_first();
    }
    ListNode *node = head;
    ListNode *previous_node;
    ListNode *to_be_removed;
    while(node->get_next_node() != NULL){
        previous_node = node;
        node = node->get_next_node();
    }
    to_be_removed = node;
    previous_node->set_next_node(NULL);
    void *element = to_be_removed->get_element();
    delete to_be_removed;
    count--;
    return element;
}

int List::get_count()
{
    return count;
}

void List::print(void (*function)(void *))
{
    ListNode *node = head;
    while(node != NULL){
        function(node->get_element());
        node = node->get_next_node();
    }
}