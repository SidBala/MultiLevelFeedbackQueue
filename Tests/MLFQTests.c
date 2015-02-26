#include <stdio.h>
#include <stdlib.h>
#include "SystemDefs.h"
#include "ProcMan.h"
#include "DevMan.h"
#include "List.h"
#include "MMU.h"
#include "MLFQ.h"

#include "Tests.h"

void DumpMLFQ()
{
	List** Qs = MLFQGetQueues();
	int numQ =0;

	Node* node = NULL;
	ProcInfo* pInfo = NULL;

	//system("clear");

	
	printf("\nMLFQ Contents\n");
	for(numQ = 0; numQ < SYSTEMMLFQLEVELS; numQ++)
	{
		printf("\n================================================================\n");
		printf("Level:%d",numQ);
		printf("\n================================================================\n");
		
		for(node = (Qs[numQ]->tail); node; node = node->prev)
		{
			pInfo = (ProcInfo*)node->data;
			printf("  |N:%d P:%d C:%d|", pInfo->num, pInfo->pri,pInfo->cpuTime);
		}
		printf("|XXX|");
	}

	
	printf("\n================================================================\n");
	//Sleep(10);
}


void TestMLFQ()
{
	ProcInfo p1 = {	0,		// num
					1,		// pri
					0,		// arrTime
					4,		// cpuTime
					128,	// memSize
					0,		// numPrint
					0,		// numScan
					0,		// numModem
					0,		// numCD
					0,		// pid
					0,		// devBlock
					0,		// memBlock
					0		// node
				  };

	Node* owner = NULL;

	List* jobList = NULL;
	ProcInfo* runProc = NULL;
	int t =0;

	

	printf("\nMLFQTests %d", sizeof(Node));

	jobList = ListCreate(sizeof(ProcInfo));

	owner = ListAppend(jobList,&p1);
	((ProcInfo*)(owner))->node = owner;

	

	p1.num = 2;
	p1.arrTime =0;
	p1.pri = 1;
	owner = ListAppend(jobList,&p1);
	((ProcInfo*)(owner))->node = owner;

	//

	p1.num = 3;
	p1.arrTime =4;
	p1.pri = 1;
	owner = ListAppend(jobList,&p1);
	((ProcInfo*)(owner))->node = owner;

	

	MMUInit(960);
	MLFQInit(jobList);
	DumpMLFQ();

	runProc  = MLFQGetNextProc(runProc,t++);
	DumpMLFQ();
	runProc->cpuTime --;
	

	runProc  = MLFQGetNextProc(runProc,t++);
	DumpMLFQ();
	runProc->cpuTime --;


	runProc  = MLFQGetNextProc(runProc,t++);
	DumpMLFQ();
	runProc->cpuTime --;

	runProc  = MLFQGetNextProc(runProc,t++);
	DumpMLFQ();
	runProc->cpuTime --;

		runProc  = MLFQGetNextProc(runProc,t++);
	DumpMLFQ();
	runProc->cpuTime --;

		runProc  = MLFQGetNextProc(runProc,t++);
	DumpMLFQ();
	runProc->cpuTime --;


		runProc  = MLFQGetNextProc(runProc,t++);
	DumpMLFQ();
	runProc->cpuTime --;


		runProc  = MLFQGetNextProc(runProc,t++);
	DumpMLFQ();
	runProc->cpuTime --;

		runProc  = MLFQGetNextProc(runProc,t++);
	DumpMLFQ();
}