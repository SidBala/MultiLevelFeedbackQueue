//=======================================
//  Multi-level Host Dispatcher
//  
//  EECE 315 - Project 4
//  Group 11
//
//  Siddhartha Balasubramanian
//  Nabil Lathiff
//  Mark Duppenthaler
//  Michael Peterson
//=======================================


//=======================================
//  List.c
//=======================================
//  A generic linked list
// 
//  Handles manipulation of linked lists
//
//  This module forms the base for the MLFQ
//  and the MMU
//
//  This list offers soft generics so that
//  consumers never have to deal with malloc
//  and free
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "List.h"

void _ListAddNew(List* list, Node* node)
//  Add a node to an empty list
{
    node->prev = NULL;
    node->next = NULL;
    node->parent = list;

    list->head  = node;
    list->tail  = node;
}

Node* _AllocNode(List* list, void* data)
// Allocate memory for a node
{
    Node* node = (Node*)malloc(sizeof(Node));

    if(!node)
        return NULL;

    if(list->dataSize)
    {
        node->data = malloc(list->dataSize);

        if(!node->data)
        {
            free(node);
            return NULL;
        }

        if(data)
            memcpy(node->data, data, list->dataSize);
    }
    else
	{
        node->data = data;
	}

	node->prev = NULL;
    node->next = NULL;
    node->parent = list;

    return node;
}

LISTERR _FreeNode(Node* node)
// Release memory for a node
{
    List* list = node->parent;
	void* data = NULL;
    
    if(!list)
        return LISTINVALID;

    if(list->dataSize && node->data)
	{
        data = node->data;
		node->data = NULL;
		free(node);
		free(data);
	}
	else
	{
		free(node);
	}
	node = NULL;

    return LISTSUCCESS;
}

List* ListCreate(int dataSize)
//  Create a new list
{
    List* list = (List*) malloc(sizeof(List));

    if(list)
    {
        list->tail = NULL;
        list->head = NULL;
        list->dataSize = dataSize;
    }

    return list;
}

LISTERR ListIsEmpty(List* list)
//  Check if the list is empty
{
    if(!list)
        return LISTINVALID;

    if(!list->head)
        return LISTEMPTY;

	return LISTSUCCESS;
}

Node* ListInsertAfter(Node* pos, void* data)
//  Insert a new node after a certain node
{
    Node* node = NULL;
    List* list = NULL;

    if(!pos || !(list = pos->parent))
        return NULL;

    node = _AllocNode(list, data);

    if(!node)
        return NULL;

    if(ListIsEmpty(list) == LISTEMPTY)
    {
        _ListAddNew(list, node);
        return node;
    }

    node->prev = pos;
    node->next = pos->next;

    if(pos->next)
        pos->next->prev = node;
    else
        list->tail = node;

    pos->next = node;

    return node;
}

Node* ListInsertBefore(Node* pos, void* data)
//  Insert a new node before a certain node
{
    Node* node = NULL;
    List* list = NULL;

    if(!pos || !(list = pos->parent))
        return NULL;

    node = _AllocNode(list, data);

    if(!node)
        return NULL;

    if(ListIsEmpty(list) == LISTEMPTY)
    {
        _ListAddNew(list, node);
        return node;
    }

    node->prev = pos->prev;
    node->next = pos;

    if(pos->prev)
        pos->prev->next = node;
    else
        list->head = node;

    pos->prev = node;

    return node;
}

Node* ListAppend(List* list, void* data)
//  Add a new node to the end of a list
{
    Node* node = NULL;

    if(!list)
        return NULL;

    node = _AllocNode(list, data);
        
    if(!node)
        return NULL;

    if(ListIsEmpty(list) == LISTEMPTY)
    {
        _ListAddNew(list, node);
        return node;
    }

    node->prev = list->tail;
    node->next = NULL;

	list->tail->next = node;
    list->tail = node;

    return node;
}

Node* ListPrepend(List* list, void* data)
// Add a new node to the beginning of a list
{
    Node* node = NULL;

    if(!list)
        return NULL;

    node = _AllocNode(list, data);
        
    if(!node)
        return NULL;

    if(ListIsEmpty(list) == LISTEMPTY)
    {
        _ListAddNew(list, node);
        return node;
    }

    node->next = list->head;
    node->prev = NULL;

	list->head->prev = node;
    list->head = node;

    return node;
}

Node* ListDetach(Node* node)
// Detach a node from a list
{
    List* list = NULL;

    if(!node)
        return NULL;

    list = node->parent;

    if(!list)
        return NULL;
    
    if(ListIsEmpty(list) == LISTEMPTY)
        return NULL;

    if(node != list->head && node!= list->tail)
    {
        if(!node->next || !node->prev)
            return NULL;

        node->next->prev = node->prev;
        node->prev->next = node->next;
    }
    else
    {
        if(node == list->head)
        {
            if(node->next)
                node->next->prev = NULL;

            list->head = node->next;
        }
        if(node == list->tail)
        {
            if(node->prev)
                node->prev->next = NULL;

            list->tail = node->prev;
        }
    }

    node->parent = NULL;

    return node;
}

Node* ListAttachFront(List* list, Node* node)
// Attach a detached node to the beginning of a list
{
    if(!list)
        return NULL;

    if(!node)
        return NULL;

    if(ListIsEmpty(list) == LISTEMPTY)
    {
        _ListAddNew(list, node);
        return node;
    }

    node->next = list->head;
    node->prev = NULL;
    node->parent = list;

    list->head->prev = node;
    list->head = node;

    return node;
}

Node* ListAttachBack(List* list, Node* node)
// Attach a detached node to the end of a list
{
    if(!list)
        return NULL;

    if(!node)
        return NULL;

    if(ListIsEmpty(list) == LISTEMPTY)
    {
        _ListAddNew(list, node);
        return node;
    }

    node->prev = list->tail;
    node->next = NULL;
    node->parent = list;

    list->tail->next = node;
    list->tail = node;

    return node;
}

LISTERR ListNodeSwap(Node* nodeA, Node* nodeB)
// Swap two nodes
{
	Node temp;
	void* dataA;
	void* dataB;

	if(!nodeA || !nodeB)
		return LISTINVALIDNODE;

	dataA = nodeA->data;
	dataB = nodeB->data;

	temp = *nodeA;

	*nodeA = *nodeB;
	*nodeB = *nodeA;

	nodeA->data = dataA;
	nodeB->data = dataB;

	return LISTSUCCESS;
}

LISTERR ListDeleteNode(Node* node)
// Delete a node from a list and free all memory
{
    List* list = NULL;

	if(!node)
        return LISTINVALIDNODE;

    list = node->parent;

    if(!list)
        return LISTINVALID;
    
    if(ListIsEmpty(list) == LISTEMPTY)
        return LISTEMPTY;

    if(node != list->head && node!= list->tail)
    {
        if(!node->next || !node->prev)
            return LISTINVALIDNODE;

        node->next->prev = node->prev;
        node->prev->next = node->next;
    }
    else
    {
        if(node == list->head)
        {
            if(node->next)
                node->next->prev = NULL;

            list->head = node->next;
        }
        if(node == list->tail)
        {
            if(node->prev)
                node->prev->next = NULL;

            list->tail = node->prev;
        }
    }

    return(_FreeNode(node));
}

LISTERR ListDelete(List* list)
// Delete all nodes from a list and free all memory
{
    Node* temp = NULL;

    if(!list)
        return LISTINVALID;

    while(temp = list->head)
    {
        list->head = list->head->next;
        
        if(_FreeNode(temp) != LISTSUCCESS)
            return LISTINVALIDNODE;
    }

    free(list);

    return LISTSUCCESS;
}
