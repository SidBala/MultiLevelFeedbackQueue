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
//	MLFQ.h
//=======================================
//	Header for Multi Level Feedback Queue
// 
//	Holds and manages scheduling of processes
//	of multiple priorities.
//	Handles resource allocation and deallocation.
//

#pragma once


#include "List.h"
#include "MMU.h"
#include "ProcMan.h"

typedef enum  
{
	MLFQSUCCESS,
    MLFQINVALIDJOBLIST,
    MLFQEMPTYJOBLIST,
    MLFQNOTINIT,
    MLFQALREADYINIT,
    MLFQAVAILABLE,
    MLFQJOBQEMPTY,
    MLFQNOMEM,
	MLFQFAIL,
    MLFQINVALIDPROC
} MLFQERR;

MLFQERR MLFQInit(List* jobList);
ProcInfo* MLFQGetNextProc(ProcInfo* prevProc, int timeStep);
MLFQERR MLFQDestroy();

MLFQERR MLFQIsJobQEmpty();

List** MLFQGetReadyQueues();
List* MLFQGetUserQueue();