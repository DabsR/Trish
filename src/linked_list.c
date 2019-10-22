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

void linkedlist_pop(LinkedList *list)
{
    assert(list->head && list->tail);

    LinkedNode *node_to_remove = NULL;

    if (list->head == list->tail)
    {
        node_to_remove = list->head;
        list->head = NULL;
        list->tail = NULL;
    }
    else
    {
        node_to_remove   = list->tail;
        list->tail       = list->tail->previous;
        list->tail->next = NULL;
    }

    if (node_to_remove)
    {
        free(node_to_remove);
    }
}

void linkedlist_remove(LinkedList *list, void *data)
{
    LinkedNode *node = list->head;
    
    while (node)
    {
        if (node->data == data)
        {
            break;   
        }
        else
        {
            node = node->next;
        }
    }

    assert(node);
    // if (node)
    {
        // We need to join the gap from removing the node.
        
        LinkedNode *previous_node = node->previous;
        LinkedNode *next_node     = node->next;
        
        // If the node is nested between two nodes.
        if (previous_node && next_node)
        {
            previous_node->next = next_node;
            next_node->previous = previous_node;
        }
        // If the node has no preceeding nodes.
        else if (next_node)
        {
            list->head = next_node;
            next_node->previous = NULL;
        }
        // If the node has no succeeding nodes.
        else if (previous_node)
        {
            list->tail = previous_node;
            previous_node->next = NULL;
        }
        // If this was the only node.
        else
        {
            list->head = NULL;
            list->tail = NULL;
        }
        

        free(node);
    }
}