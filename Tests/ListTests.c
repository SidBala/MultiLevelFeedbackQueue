#include <stdio.h>
#include <stdlib.h>

#include "Tests.h"
#include "List.h"

void TestList()
{
	int a,b,c;
	Node* ite = NULL;
	
	List* newList = ListCreate(sizeof(int));

	a = 10;
	b = 20;
	c = 30;

	ListAppend(newList, &a);
	ListAppend(newList, &b);
	ListAppend(newList, &c);

	ite = newList->head;
	printf("\n");
	for(ite = newList->head; ite != NULL; ite = ite->next)
	{
		printf("%d\t",*(int*)(ite->data));
	}


	a = 40;
	b = 50;
	c = 60;

	ListPrepend(newList, &a);
	ListPrepend(newList, &b);
	ListPrepend(newList, &c);

	printf("\n");

	ite = newList->head;
	for(ite = newList->head; ite != NULL; ite = ite->next)
	{
		printf("%d\t",*(int*)(ite->data));
	}

	ListDeleteNode(ite);

	ite = newList->head;
	while(*(int*)ite->data != 10) ite=ite->next;
	ListDeleteNode(ite);


	printf("\n");
	ite = newList->head;
	do 
	{
		printf("%d\t",*(int*)(ite->data));
	}while(ite = ite->next);

	
	do
	{
		ListDeleteNode(newList->head);

		printf("\n");
		ite = newList->head;

		for(ite = newList->head; ite != NULL; ite = ite->next)
		{
			printf("%d\t",*(int*)(ite->data));
		}

	}while(ListIsEmpty(newList) == LISTSUCCESS);


	ListDelete(newList);

	return;
}