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
//	Visualizer.c
//=======================================
//	State visualizer
// 
//	Given the instance to a MMU/MLFQ, prints
//	out information on to console UI
//

#include <stdio.h>

#include "Visualizer.h"
#include "SystemDefs.h"
#include "DevMan.h"

int runningProc;

void VisChangeColor(VISCOL colf, VISCOL colb, VISSTYLE style)
// Change console background and foreground colors and style
{
	char str[13];

	sprintf(str, "%c[%d;%d;%dm", 0x1B, style, colf + 30, colb + 40);
	printf("%s", str);
}

void VisResetColor(void)
// Reset the color to the default
{
	VisChangeColor(WHITE, BLACK, RESET);
}

void VisClearScreen(void)
// Clear the screen
{
	printf("\033[2J");
}

void VisSetCursorPos(int x, int y)
// Set cursor position
{
	printf("\033[%d;%d",x,y);
}

void _PrintMFLQ(void)
// Print the state of the MLFQ
// Output the process number and remaining cpu time of eachprocess
// at every level of the queue
// The process block is displayed as |#:n Cpu:t| where n is the number and t is the cpu time remaining

// The process that is running currently is displayed in green
// The processes that are suspended currently are displayed in yellow
// The processes that cannot run due to resource contention are displayed in red
// The processes that have not been started are displayed in grey
{
	List** Qs = MLFQGetReadyQueues();
	List* uQ = MLFQGetUserQueue();
	int numQ =0;

	Node* node = NULL;
	ProcInfo* pInfo = NULL;

	VisChangeColor(WHITE,BLACK,BRIGHT);

	printf("\nMulti Level Feedback Queue Contents. Color code shown below");

	VisChangeColor(GREEN,BLACK,BRIGHT);
	printf("\n[X] Process Running\t\t\t");

	VisChangeColor(YELLOW,BLACK,BRIGHT);
	printf("[X] Process Suspended\n");

	VisChangeColor(RED,BLACK,BRIGHT);
	printf("[X] Resources Unavailable\t\t");
	VisResetColor();

	VisChangeColor(MAGENTA,BLACK,BRIGHT);
	printf("[X] Blocked by Higher Priority\n");
	VisResetColor();

	// Print the user queue
	VisChangeColor(WHITE,BLACK,BRIGHT);
	printf("\nUser Dispatch Queue");
	VisResetColor();
	printf("\n-------------------------------------------------------------------------------\n");
		
	for(node = (uQ->tail); node; node = node->prev)
	{
		pInfo = (ProcInfo*)node->data;
			if(pInfo->status == PROCPRIORITYDENY)
			 	VisChangeColor(MAGENTA, BLACK, BRIGHT);
			else if(pInfo->status == PROCWAITRESOURCES)
				VisChangeColor(RED, BLACK, BRIGHT); 

		printf("|#%d C:%d P:%d|\t ", pInfo->num, pInfo->cpuTime, pInfo->pri);
		VisResetColor();
	}
	printf("| HEAD |");
	printf("\n-------------------------------------------------------------------------------\n");

	// Print the queues level by level
	for(numQ = 0; numQ < SYSTEMMLFQLEVELS; numQ++)
	{
		VisChangeColor(WHITE,BLACK,BRIGHT);
		printf("\nPriority Level:%d",numQ);
		VisResetColor();
		printf("\n-------------------------------------------------------------------------------\n");
		
		for(node = (Qs[numQ]->tail); node; node = node->prev)
		{
			pInfo = (ProcInfo*)node->data;
				if(pInfo->status == PROCRUNNING)
				{
					runningProc = pInfo->num;
				 	VisChangeColor(GREEN, BLACK, BRIGHT);
				}
				else if(pInfo->status == PROCSUSPENDED)
				 	VisChangeColor(YELLOW, BLACK, BRIGHT);
				else if(pInfo->status == PROCWAITRESOURCES)
					VisChangeColor(RED, BLACK, BRIGHT); 

			printf("|#%d Cpu:%d|\t ", pInfo->num, pInfo->cpuTime);
			VisResetColor();
		}
		printf("| HEAD |");
		printf("\n-------------------------------------------------------------------------------\n");
	}

}

void _PrintMMU(void)
// Prints the state of the memory manager
// Each block represents a memory block.
// Each block indicates the owner of the block
// If the block is free, that is indicated too
// If the owner of the block is currently running, the block is displayed in green
// If the owner of the block is currently suspended, the block is displayed in yellow
// Free blocks are displayed in grey
{
	MMU* myMMU = GetMMU();
	List* memList = myMMU->storage;
	Node* memNode = memList->head;
	MemBlock* memBlock = NULL;
	char owner[10];

	VisChangeColor(WHITE,BLACK,BRIGHT);
	printf("\n\nUser Memory Contents - shows individual blocks of memory");
	VisResetColor();

	printf("\n--------------------------------------------");
	for(;memNode!= NULL; memNode = memNode->next)
	{
		memBlock = (MemBlock*)memNode->data;
		
		if(memBlock->owner)
		{
			if(memBlock->owner == runningProc)
				VisChangeColor(GREEN,BLACK,BRIGHT);
			else
				VisChangeColor(YELLOW,BLACK,BRIGHT);

			printf("\n| Owner:%4d Start:%3d End:%3d Size: %3dMB |",(int)memBlock->owner,memBlock->start, memBlock->end,(memBlock->end-memBlock->start)+1);
			VisResetColor();
		}
		else
			printf("\n| FREESPACE  Start:%3d End:%3d Size: %3dMB |",memBlock->start, memBlock->end,(memBlock->end-memBlock->start)+1);

		printf("\n--------------------------------------------");
	}
	

}

void _PrintDevices(void)
// Displays the number of free devices available in the system
{
	DeviceState* dev = DevManGetDeviceState();

	VisChangeColor(WHITE,BLACK,BRIGHT);
	printf("\n\nDevice Status - shows free devices");
	printf("\n-------------------------------------------------------------------------------");
	VisResetColor();

	printf("\nCD Drives:%d\tScanners:%d\tPrinters:%d\tModems:%d\n",dev->cd,dev->scan,dev->print,dev->modem);
}

void _PrintProcMessages(void)
// This is a header that indicates where the process messages should be printed.
// Note: this is not always effective. Due to timing issues, sometimes, the process messages may not 
// be printed exactly below this header. But this happens rarely
{
	VisChangeColor(WHITE,BLACK,BRIGHT);
	printf("\n\nAny messages from the processes are displayed below\n");
	printf("-------------------------------------------------------------------------------\n");
	VisResetColor();
}

void VisPrintState(void)
// This grand function displays the state of the entire system
{
	VisClearScreen();

	_PrintMFLQ();
	_PrintDevices();
	_PrintMMU();
	_PrintProcMessages();

	fflush(stdout);
}