#ifndef LINKED_LIST_H
#define LINKED_LIST_H 1

typedef struct LinkedNode
{
    void *data;
    struct LinkedNode *previous;
    struct LinkedNode *next;

} LinkedNode;

typedef struct LinkedList
{
    LinkedNode *head;
    LinkedNode *tail;
    
} LinkedList;



LinkedList * linkedlist_create();
void         linkedlist_free(LinkedList *list);
void         linkedlist_push(LinkedList *list, void *data);

#endif