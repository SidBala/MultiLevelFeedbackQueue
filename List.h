//=======================================
//	Multi-level Host Dispatcher
//	
//	EECE 315 - Project 4
//  Group 11
//
//	Siddhartha Balasubramanian
//	Nabil Lathiff
//	Mark Duppenthaler
//	Michael Peterson
//=======================================


//=======================================
//	List.h
//=======================================
//	Header for generic linked list module
// 
//	Handles manipulation of linked lists
//
//  This module forms the base for the MLFQ
//  and the MMU
//

#pragma once

typedef struct _List List;
typedef struct _Node Node;

struct _List
{
    Node* head;
    Node* tail;
    int dataSize;
};

struct _Node
{
    void* data;
    Node* next;
    Node* prev;
    List* parent;
};

typedef enum  
{
    LISTSUCCESS,
    LISTNOMEM,
    LISTINVALID,
    LISTINVALIDNODE,
    LISTEMPTY
} LISTERR;

List* ListCreate(int dataSize);

LISTERR ListIsEmpty(List* list);

Node* ListInsertAfter(Node* pos, void* data);
Node* ListInsertBefore(Node* pos, void* data);

Node* ListAppend(List* list, void* data);
Node* ListPrepend(List* list, void* data);

Node* ListDetach(Node* node);
Node* ListAttachFront(List* list, Node* node);
Node* ListAttachBack(List* list, Node* node);

LISTERR ListNodeSwap(Node* nodeA, Node* nodeB);

LISTERR ListDeleteNode(Node* node);
LISTERR ListDelete(List* list);