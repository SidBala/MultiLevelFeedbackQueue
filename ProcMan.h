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
//	ProcMan.h
//=======================================
//	Header for Process Manager
// 
//  Interacts with the MLFQ to handle
//	starting/stoping/pausing of processes.
//

#pragma once

#include "List.h"
#include "MMU.h"

typedef struct _ProcInfo ProcInfo;

typedef enum  
{
    PROCMANSUCCESS,
    PROCMANWAITING,
    PROCMANALLDONE,
    PROCINVALIDBLOCK,
    
} PROCERR;

typedef enum
{
    PROCNOTRUNNING,
    PROCALLOCATED,
    PROCPRIORITYDENY,
    PROCRUNNING,
    PROCSUSPENDED,
    PROCTERMINATED,
    PROCWAITRESOURCES
} PROCSTATUS;

struct _ProcInfo
{
    int num;
    int pri;
    int arrTime;
    int cpuTime;
    int memSize;
    int numPrint;
    int numScan;
    int numModem;
    int numCD;

    int pid;

    int devBlock;
    MemBlock* memBlock;

    PROCSTATUS status;
  
    Node* node;
};

PROCERR ProcManRun(int timeStep);
Node* ProcManLinkNode(Node* node);
