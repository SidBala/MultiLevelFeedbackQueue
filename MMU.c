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
//	MMU.c
//=======================================
//	Memory Management Unit
// 
//	Allocates contigious blocks of free memory.
//	Uses the best fit algorithm to find free blocks.
//	Releases memory at process exit.
//	Tracks free blocks and manages memory.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "MMU.h"

MMU* theMMU;

MMUERR MMUInit(int memSize)
// Initialize the Memory Manager to a specified size
{
	MemBlock memBlock;
	Node* memNode = NULL;

	if(memSize < 1)
		return MMUINVALIDARG;

	if(theMMU)
		return MMUALREADYEXISTS;

	theMMU = (MMU*)malloc(sizeof(MMU));	// Create the MMU structure

	if(!theMMU)
		return MMUNOMEM;

	theMMU->storage = ListCreate(sizeof(MemBlock));	// The MMU's storage is a doubly-linked linked list

	if(!theMMU->storage)
	{
		free(theMMU);
		return MMUNOMEM;
	}

	// Create the first memory block in the MMU. This is a free block of memory that spans the entire available memory
	memBlock.owner = 0;
	memBlock.start = 0;
	memBlock.end = (memBlock.start + memSize) -1;

	if(!(memNode = ListPrepend(theMMU->storage, &memBlock)))
	{
		free(theMMU);
		return MMUNOMEM;
	}

	((MemBlock*)memNode->data)->node = memNode;
	
	theMMU->size = memSize;

	return MMUSUCCESS;
}

MMUERR MMUCheckAllocate(int size)
// Check if allocation of a block of the required size is possible or not
{
	Node* memNode = NULL;
	MemBlock* memBlock = NULL;

	Node* targetNode = NULL;

	int minBlockSize = INT_MAX;
	int blockSize = 0;

	if(size<1)
		return MMUINVALID;

	memNode = theMMU->storage->head;

	// This loop searches for a memory block based on the best fit algorithm
	for(; memNode ; memNode=memNode->next)
	{
		memBlock = (MemBlock*)memNode->data;
		
		if(memBlock->owner)
			continue;

		blockSize = (memBlock->end - memBlock->start) + 1;

		if(blockSize >= size && blockSize < minBlockSize)
		// If a valid block was found
		{
			minBlockSize = blockSize;
			targetNode = memNode;
		}
	}

	if(targetNode)
		return MMUAVAILABLE;
	else
		return MMUNOMEM;

}

MemBlock* MMUAllocate(int size)
// Allocate memory for a process of the required size
{
	Node* memNode = NULL;
	MemBlock* memBlock = NULL;

	Node* targetNode = NULL;
	MemBlock* targetBlock = NULL;

	MemBlock newBlock;

	int minBlockSize = INT_MAX;
	int blockSize = 0;

	if(size<1)
		return NULL;

	memNode = theMMU->storage->head;

	// Search for a free block using best fit
	for(; memNode!= NULL; memNode=memNode->next)
	{
		memBlock = (MemBlock*)memNode->data;
		
		if(memBlock->owner)
			continue;

		blockSize = (memBlock->end - memBlock->start) + 1;

		if(blockSize >= size && blockSize < minBlockSize)
		// If the block is found
		{
			minBlockSize = blockSize;
			targetNode = memNode;
		}
	}

	if(!targetNode)
		return NULL;

	// We now create make a new memory block here
	targetBlock = (MemBlock*)targetNode->data;

	if(minBlockSize == size)
	// If the block size matches the required size, just turn the free block into a used block.
	{
		targetBlock->owner = -1;
		return targetBlock;
	}

	// If the free block is of greater size than the required size, create a new block just before
	// the free block and adjust the free block's size
	newBlock.start = targetBlock->start;
	newBlock.end = (targetBlock->start + size) - 1;
	newBlock.owner = -1;

	targetBlock->start = newBlock.end + 1;

	memNode = ListInsertBefore(targetNode, &newBlock);

	memBlock = (MemBlock*) memNode->data;
	memBlock->node = memNode;

	return memBlock;
}

MMUERR MMURelease(MemBlock* memBlock)
// Releases memory for a process 
// If there are adjacent free blocks, this function combines these blocks into a single free block
{
	Node* memNode = NULL;

	Node* targetNode = NULL;
	MemBlock* targetBlock = NULL;

	memNode = memBlock->node;

	if(memNode->prev)
	{
		targetNode = memNode->prev;
		targetBlock = (MemBlock*)targetNode->data;

		if(!targetBlock->owner)
		// If the block before this block is free, we combine this block with that one
		{
			memBlock->start = targetBlock->start;

			if(!ListDeleteNode(targetNode) == LISTSUCCESS)
				return MMUFAIL;
		}
	}

	if(memNode->next)
	{
		targetNode = memNode->next;
		targetBlock = (MemBlock*)targetNode->data;

		if(!targetBlock->owner)
		// If the block after this block is free, we combine this block with that one
		{
			memBlock->end = targetBlock->end;
			
			if(!ListDeleteNode(targetNode) == LISTSUCCESS)
				return MMUFAIL;
		}
	}

	// Mark this block as being free now
	memBlock->owner = 0;

	return MMUSUCCESS;
}

MMU* GetMMU()
// Returns the mmu structure. This is used for printing the state
{
	return theMMU;
}

MMUERR MMUDestroy()
// Destroy the mmu structure and free all memory
{
	if(!theMMU)
		return MMUINVALID;

	if(ListDelete(theMMU->storage) != LISTSUCCESS)
		return MMUDESTROYFAIL;

	free(theMMU);
	theMMU = NULL;

	return MMUSUCCESS;
}