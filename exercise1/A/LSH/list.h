class ListNode
{
    private:
        void* element;
        ListNode* next_node;

    public:
        ListNode(void *);
        ~ListNode();

        void* get_element();

        ListNode* get_next_node();
        void set_next_node(ListNode *);
};

class List
{
    private:
        ListNode* head;
        int count;

    public:
        List();
        ~List();
        // Deletes list and nodes, but not their contents.

        void insert_first(void *);
        void insert_last(void *);

        void* remove_first();
        void* remove_last();

        int get_count();

        void print(void (*)(void *));
};