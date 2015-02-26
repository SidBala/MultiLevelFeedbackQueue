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
//	MMU.h
//=======================================
//	Header for Memory Management Unit
// 
//	Allocates contigious blocks of free memory.
//	Releases memory at process exit.
//	Tracks free blocks and manages memory.
//

#pragma once

#include "List.h"

typedef struct _MemBlock MemBlock;
typedef struct _MMU MMU;

typedef enum  
{
	MMUSUCCESS,
    MMUALREADYEXISTS,
    MMUFAIL,
    MMUDESTROYFAIL,
    MMUAVAILABLE,
	MMUNOMEM,
    MMUINVALIDARG,
	MMUINVALID,
    MMUFULL,
} MMUERR;

struct _MemBlock
{
	int owner;
    int start;
    int end;
	Node* node;
};

struct _MMU
{
    int size;
    List* storage;
};

MMUERR MMUInit(int memSize);

MMUERR MMUCheckAllocate(int size);
MemBlock* MMUAllocate(int size);

MMUERR MMURelease(MemBlock* memBlock);

MMU* GetMMU();

MMUERR MMUDestroy();