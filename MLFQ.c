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
//	MLFQ.c
//=======================================
//	Multi Level Feedback Queue
// 
//	Holds and manages scheduling of processes
//	of multiple priorities.
//	Handles resource allocation and deallocation.
//

#include <stdio.h>
#include <stdlib.h>

#include "SystemDefs.h"
#include "List.h"
#include "ProcMan.h"
#include "MMU.h"
#include "DevMan.h"
#include "MLFQ.h"


List* jobQ;
List* userQ;
List* readyQ [SYSTEMMLFQLEVELS];

int MLFQisInit;

MLFQERR _AdmitProcs(int timeStep)
// Admit processes into the ready queues based on their priority and arrival times
{
	Node* procNode = NULL;
	Node* thisNode = NULL;
	ProcInfo* info = NULL;

	if(ListIsEmpty(jobQ) == LISTEMPTY)
		return MLFQEMPTYJOBLIST;

	procNode = jobQ->head;

	// Iterate through all the processses in the job queue
	while(procNode)
	{
		thisNode = procNode;
		info = (ProcInfo*)thisNode->data;	
		
		procNode = procNode->next;

		if(info->arrTime <= timeStep)
		{
			if(info->pri > SYSTEMMLFQLEVELS - 1)
				return MLFQINVALIDPROC;

			// If the process is a real time one, add it directly to the RQ
			if(info->pri == 0)
			{
				thisNode = ListDetach(thisNode);
				ListAttachBack(readyQ[0], thisNode);
			}
			else
			// Or else admit the process into the user queue
			{
				thisNode = ListDetach(thisNode);
				ListAttachBack(userQ, thisNode);
			}
		}
	}

	return MLFQSUCCESS;
}

MLFQERR MLFQInit (List* jobList)
//	Initialize the multi level feedback queue
{
	int i = 0;

	if(!jobList)
		return MLFQINVALIDJOBLIST;

	if(ListIsEmpty(jobList) != LISTSUCCESS)
		return MLFQEMPTYJOBLIST;

	if(MLFQisInit)
		return MLFQALREADYINIT;

	if(!(userQ = ListCreate(sizeof(ProcInfo))))
		return MLFQNOMEM;

	jobQ = jobList;

	// Create the ready queue levels
	for(i = 0; i <SYSTEMMLFQLEVELS; i ++)
	{
		if(!(readyQ[i] = ListCreate(sizeof(ProcInfo))))
		{
			for(i = i - 1; i >= 0; i--)
			{
				ListDelete(readyQ[i]);
			}
			ListDelete(userQ);

			return MLFQNOMEM;
		}

	}
	MLFQisInit = 1;
	return MLFQSUCCESS;
}

MLFQERR MLFQJobQEmpty()
//	Check if the job queue is empty
{
	if(ListIsEmpty(jobQ) == LISTEMPTY)
		return MLFQJOBQEMPTY;
	else
		return MLFQAVAILABLE;
}
ProcInfo* MLFQGetNextProc(ProcInfo* prevProc, int timeStep)
//	Returns the next process to execute from the MLFQ
{
	ProcInfo* procInfo = NULL;
	Node* nextProc = NULL;
	Node* thisProc = NULL;

	int qNum = 0;
	int lowestLevel = SYSTEMMLFQLEVELS - 1;

	Node* retNode = NULL;

	// If we had a process executing before, we first check if it was done
	// If it was done, we release resources
	// If the process executing was a priority 0 (real time process), let it continue.
	// If it was any other priority, demote it if there is another process with the same or higher priority
	if(prevProc)
	{	
		// If the process is not done yet
		if(prevProc->cpuTime)
		{
			// If the process is realtime
			if(prevProc->pri == 0)
				return prevProc;	// Let it continue executing
			else
			{	
				// If the process is not realtime, and there is another process of the same priority waiting
				if(prevProc->node->next)
				{
					// Demote its priority if it is not at the lowest priority level yet 
					if(prevProc->pri != SYSTEMMLFQLEVELS - 1)
						prevProc->pri ++;

					// Remove the process from the head and reattach to the tail of the ready queue.
					ListDetach(prevProc->node);
					ListAttachBack(readyQ[prevProc->pri],prevProc->node);
				}
				else
				{
					// If there is no other process waiting at the same priority as the last process, continue with it
					return prevProc;
				}
			}
		}
		else
		// If the process that last executed is done with its work
		{
			// If it was not a real time proc, we need to release the resources it was using
			if(prevProc->pri != 0)
			{
				// Release hardware and memory
				DevManRelease(prevProc);
				MMURelease(prevProc->memBlock);
			}
			// After releasing resources, delete it from the ready queue
			ListDeleteNode(prevProc->node);
		}
	}
	// Now we are done with handling the previously executed process


	//	Admit processes based on the time
	_AdmitProcs(timeStep);


	// Now we allocate resources for user processes.
	// The general rule is that no process that is of a lower priority will be allowed to
	// acquire resources if a higher process was denied resources
	if(ListIsEmpty(userQ) != LISTEMPTY)
	{
		// Mark every process as priority denied
		// We will later allocate resources and change this stauts
		nextProc = userQ->head;
		while(nextProc)
		{
			procInfo = (ProcInfo*)nextProc->data;
			procInfo->status = PROCPRIORITYDENY;
			nextProc= nextProc->next;
		}

		// Here we allocate resources to the highest priority process first
		for(qNum = 1; qNum <= lowestLevel; qNum++)
		{
			nextProc = userQ->head;
			while(nextProc)
			{
				procInfo = (ProcInfo*)nextProc->data;
				thisProc = nextProc;
				nextProc = nextProc->next;

				// Check only processses of this priority
				if(procInfo->pri != qNum)
				{
					continue;
				}

				if(DevManCheckAllocate(procInfo) == DEVMANAVAILABLE 
					&& MMUCheckAllocate(procInfo->memSize) == MMUAVAILABLE)
				{
					// Allocation will only take place if there is no other process of higher priority
					// That is without resources
					procInfo->memBlock = MMUAllocate(procInfo->memSize);
					procInfo->memBlock->owner = procInfo->num;

					procInfo->devBlock = DevManAllocate(procInfo);

					// Load this guy into the ready queue now
					thisProc = ListDetach(thisProc);
					ListAttachBack(readyQ[procInfo->pri], thisProc);
					procInfo->status = PROCALLOCATED;
				}
				else
				// A process at this priority has been refused resource allocation
				// No process of lower priority shall get allocation
				{
					procInfo->status = PROCWAITRESOURCES;
					lowestLevel = qNum;
				}	
			}
		}
	}


	// Here we determine what is the next process to execute

	if(ListIsEmpty(readyQ[0]) != LISTEMPTY)
		// If the realtime queue has processes, execute the first one in this queue.
		return (ProcInfo*)readyQ[0]->head->data;

	// If there are no realtime processes, search for the next process to execute from the user queues
	// We need to find the first process of highest priority
	for(qNum = 1; qNum<SYSTEMMLFQLEVELS; qNum++)
	{
		nextProc = readyQ[qNum]->head;
		
		if(!nextProc)
			continue;

		return (ProcInfo*)nextProc->data;
	}

	// If we cannot find any process to execute return null
	// This can only happen if we are done will all processes in which case, the jobQ will also be empty
	// of if the jobQ is not empty, we have jobs that are still not admitted.
	return NULL;
}

MLFQERR MLFQIsJobQEmpty()
// Check if the job queue is empty.
// This is used by the process manager do determine if all jobs are done 
{
	if(ListIsEmpty(jobQ) == LISTEMPTY)
		return MLFQJOBQEMPTY;
	else
		return MLFQSUCCESS;
}

List** MLFQGetReadyQueues()
// Returns the array of ready queues. Used for printing the state of the ready queues
{
	return readyQ;
}

List* MLFQGetUserQueue()
// Returns the user queue. Used for printing the state of the user queue
{
	return userQ;
}