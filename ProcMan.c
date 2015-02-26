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
//	ProcMan.c
//=======================================
//	Process Manager
// 
//  Interacts with the MLFQ to handle
//	starting/stoping/pausing of processes.
//	

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

#include "List.h"
#include "MLFQ.h"
#include "ProcMan.h"
#include "Proc.h"

ProcInfo* lastProc;

void _SuspendProc(ProcInfo* proc)
// Suspends a process by sending a SIGTSTP
{
	proc->status = PROCSUSPENDED;
	
	if(proc->pid)
	{
		kill(proc->pid,SIGTSTP);
	}
}

void _WakeProc(ProcInfo* proc)
// Resumes a process by sending a SIGCONT
{
	proc->status = PROCRUNNING;

	if(proc->pid)
		kill(proc->pid,SIGCONT);
}

void _StartProc(ProcInfo* proc)
// Stats a new process using fork.
// The new process then executes the ProcFunc() function
{
	int retVal = 0;
	retVal = fork();

	if(retVal < 0)
		printf("\nError starting process %d", proc->num);
	if(retVal == 0)
	// If child, execute the proc function
	{
		ProcFunc(proc);
	}
	else
	// If parent
	{
		// Store the PID and mark the process as running
		proc->pid = retVal;
		proc->status = PROCRUNNING;
	}	
}

int _isProcAlive(ProcInfo* proc)
// Checks if the process is still alive.
// This can be used to see if the process was terminated due to some external reasons
{
	if(proc->pid)
	{
		if(kill(proc->pid,0) == 0)
			return 1;
	}
	else
		return 0;
}

void _KillProc(ProcInfo * proc)
// Terminates a process by sending a SIGINT
{
	if(proc->pid)
	{
		kill(proc->pid,SIGINT);

		// Mark the process as not running.
		proc->pid = 0;
		proc->status = PROCNOTRUNNING;
	}
}

PROCERR ProcManRun(int timeStep)
// This function retrieves the next process to execute from the MLFQ and decides what to do with the previous process
// After this decision, it suspends or terminates the previous process and then resumes or starts the new process
{
	ProcInfo* nextProc = NULL;
	int procKilled = 0;
	
	// If the last executing process is done executing or is dead due to external reasons
	// kill it and remove it from the scheduling queue
	if(lastProc && ( !lastProc->cpuTime || !_isProcAlive(lastProc)))
	{
		_KillProc(lastProc);
		lastProc->cpuTime = 0;
		procKilled = 1;
	}

	// Get the next process to execute from the MLFQ
	nextProc = MLFQGetNextProc(lastProc,timeStep);

	if(procKilled)
	// If the last process was killed, mark it as null
	{
		procKilled = 0;
		lastProc = NULL;
	}

	// Delete all processes returned from the MLFQ that are dead due to external reasons
	while( nextProc && nextProc->pid && !_isProcAlive(nextProc))
	{
		_KillProc(nextProc);
		nextProc->cpuTime = 0;
		nextProc = MLFQGetNextProc(nextProc,timeStep);
	}

	// At this point, we have found a valid process to execute (if nextproc is not NULL)
	// or we have determined that we must wait this quantum (if nextproc is NULL)
	if(nextProc)
	{
		if(lastProc)
		// If the next process to execute is the same as the last process
		// do nothing. Let the same process continue executing. Only decrement the cpu time.
		{
			if(nextProc == lastProc)
			{
				nextProc->cpuTime--;
				return PROCMANSUCCESS;
			}
			else
			{
				// If the last process is different from the next process and if the last process
				// was already started, suspend it.
				if(lastProc->pid)
					_SuspendProc(lastProc);
			}
		}
	
		// If the last process was already started some time ago, resume it. Otherwise, start it.
		if(nextProc->pid)
			_WakeProc(nextProc);
		else
			_StartProc(nextProc);

		// Update the last process and decrement the cpu time.
		lastProc = nextProc;
		nextProc->cpuTime --;
		return PROCMANSUCCESS;
	}
	else
	{
		if(MLFQIsJobQEmpty() == MLFQJOBQEMPTY)
			return PROCMANALLDONE;
		else
			return PROCMANWAITING;
	}
}

Node* ProcManLinkNode(Node* node)
// This is required for generic use of the linked list to store the process structures
{
	ProcInfo* procInfo;
	if(!node || !(node->data))
		return NULL;

	procInfo = (ProcInfo*) node->data;

	procInfo->node = node;

	return node;
}