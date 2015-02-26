#include <stdio.h>
#include <stdlib.h>

#include "Tests.h"
#include "MMU.h"
#include "List.h"

void DumpMMU()
{
	MMU* myMMU = GetMMU();
	List* memList = myMMU->storage;
	Node* memNode = memList->head;
	MemBlock* memBlock = NULL;

	printf("\n\nMemory DataStore DUMP");
	for(;memNode!= NULL; memNode = memNode->next)
	{
		memBlock = (MemBlock*)memNode->data;
		printf("\n------------------");
		printf("\nOw:%5d S:%5d\nSz:%5d E:%5d ",(int)memBlock->owner,memBlock->start,(memBlock->end-memBlock->start)+1, memBlock->end);
	}
	printf("\n==================");
}

void TestMMU()
{
	MemBlock *bl1,*bl2,*bl3,*bl4,*bl5,*bl6,*bl7,*bl8;
	int b;
	MMU* myMMU = NULL;

	MMUInit(1024);
	myMMU = GetMMU();
		

	
	b=0;

	b  = ((MemBlock*)myMMU->storage->head->data)->end;

	printf("1023==%d",b);

	DumpMMU();

	bl1 = MMUAllocate(62);
	bl1->owner = 1;

	bl2 = MMUAllocate( 32);
	bl2->owner = 2;

	bl3 = MMUAllocate( 12);
	bl3->owner = 3;

	DumpMMU();

	MMURelease(  bl2);
	
	DumpMMU();

	bl4 = MMUAllocate( 12);
	bl4->owner = 4;

	DumpMMU();

	bl5 = MMUAllocate( 18);
	bl5->owner = 5;

	DumpMMU();

	bl6 = MMUAllocate( 2);
	bl6->owner = 6;

	DumpMMU();

	MMURelease( bl6);
	DumpMMU();

	MMURelease( bl3);
	DumpMMU();

	MMURelease( bl5);
	DumpMMU();

	MMURelease( bl1);
	DumpMMU();

	MMURelease( bl1);
	DumpMMU();

	MMURelease( bl5);
	DumpMMU();

	MMURelease( bl4);
	DumpMMU();

	bl7 = MMUAllocate( 1020);
	bl7->owner = 7;
	DumpMMU();

	if(bl8 = MMUAllocate(2010))
	{
		bl8->owner = 8;
		printf("\nNot supposed to see this");
	}
	DumpMMU();
	
	MMURelease( bl7);
	DumpMMU();
	printf("\nShould only see one huge block now");

	MMUDestroy();

	
	return;
}