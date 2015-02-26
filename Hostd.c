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
//	Hostd.c
//=======================================
//	Main for the HOST Dispatcher
//


#include <stdio.h>
#include <stdlib.h>

#include "SystemDefs.h"

#include "List.h"
#include "ProcMan.h"
#include "MMU.h"
#include "FileLoader.h"
#include "DevMan.h"
#include "MLFQ.h"
#include "Visualizer.h"

int main(int argc, char* argv[])
{
	List* jobList = NULL;
	int time = 0;

	if(argc != 2)
	{
		printf("\nInvalid number of arguments\n");
		return EXIT_FAILURE;
	}

	if(FileExists(argv[1]) != FILESUCCESS)
	{
		printf("\nInput file could not be found\n");
		return EXIT_FAILURE;
	}

	if(!(jobList = FileLoadJobs(argv[1])))	// Read the job list from file
	{
		printf("\nUnable to read from input file\n");
		return EXIT_FAILURE;
	}
	
	if(MMUInit(SYSTEMMEMSIZE - SYSTEMRTRESERVEMEM) != MMUSUCCESS)	// Initialize the memory management unit
		return EXIT_FAILURE;									

	if(MLFQInit(jobList) != MLFQSUCCESS)	// Initialize the multi level feedback queue
		return EXIT_FAILURE;	

	if(DevManInit() != DEVMANSUCCESS) // Initialize the device manager
		return EXIT_FAILURE;

	while(ProcManRun(time++) != PROCMANALLDONE)		// Run the simulation until all jobs are complete
	{
		VisPrintState();	// Print the state of the system
		sleep(SYSTEMTIMEQUANT);	// Sleep one time quantum
	}

	printf("\nAll jobs completed. Exiting.\n");

	// Release all resources and exit
	MMUDestroy();
	ListDelete(jobList);
	
	return EXIT_SUCCESS; 
}