#include "linked_list.h"
#include "utilities.h"

LinkedList * linkedlist_create()
{
    LinkedList *list = allocate(LinkedList);

    return list;
}

void linkedlist_free(LinkedList *list)
{
    LinkedNode *node = list->tail;

    while (node)
    {
        LinkedNode *next_node = node->previous;
        free(node);
        node = next_node;
    }

    free(list);
}

void linkedlist_push(LinkedList *list, void *data)
{
    LinkedNode *node = allocate(LinkedNode);
    node->data       = data;
    
    if (list->tail)
    {
        node->previous   = list->tail;
        list->tail->next = node;

        list->tail = node;
    }
    else
    {
        list->head = node;
        list->tail = node;
    }
}