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
//	FileLoader.c
//=======================================
//	FileLoader module
// 
//	Loads the job lists from a specified 
//	file.
//

#include <stdio.h>
#include <stdlib.h>

#include "SystemDefs.h"

#include "FileLoader.h"
#include "List.h"
#include "ProcMan.h"

FILEERR FileExists(char* filename)
// Check if the file exists
{
	FILE * inputfile = fopen( filename, "r" );

	if(!inputfile)
		return FILEDOESNOTEXIST;

	fclose(inputfile);

	return FILESUCCESS;
}

List* FileLoadJobs(char* filename)
// Load all the jobs from the file list
// This function also validates each job to check if the resources it requires
// can be provided by this machine
// If it cannot, that job is not admitted into the joblist 
{
	ProcInfo procInfo;
	List* jobList;
	int procNum = 1;

	FILE * inputfile = fopen( filename, "r" );

	if(!inputfile)
		return NULL;

	if(!(jobList = ListCreate(sizeof(ProcInfo))))
	{
		fclose(inputfile);
		return NULL;
	}

	// Read the file here and add the processes to the job list
	while( feof(inputfile) == 0 )
	{
	    if(fscanf(inputfile, "%d, %d, %d, %d, %d, %d, %d, %d",
	        &(procInfo.arrTime),
	        &(procInfo.pri),
	        &(procInfo.cpuTime),
	        &(procInfo.memSize),
	        &(procInfo.numPrint),
	        &(procInfo.numScan),
	        &(procInfo.numModem),
	        &(procInfo.numCD)) != 8)

	    	// If the scanf cannot scan the 8 required parameters for this job, go to the next job
	    	// Ideally, if the input file is according to spec, this will never happen.
        	continue;

        // Validate all the parameters to check if the process can execute on this machine
        // Note, this eliminates process that can never run on this computer.
        // Eg: a process requiring 3 CD drives on a machine that has only 2 will be eliminated
		if(procInfo.arrTime < 0)
			continue;
		if(procInfo.cpuTime <= 0)
			continue;
		if(procInfo.pri < 0	 || procInfo.pri > (SYSTEMMLFQLEVELS - 1))
			continue;
		if(procInfo.memSize < 0	 || procInfo.memSize > (SYSTEMMEMSIZE - SYSTEMRTRESERVEMEM))
			continue;
		if(procInfo.numPrint < 0 || procInfo.numPrint > SYSTEMNUMPRINT)
			continue;
		if(procInfo.numScan < 0	|| procInfo.numScan > SYSTEMNUMSCAN)
			continue;
		if(procInfo.numModem < 0 || procInfo.numModem > SYSTEMNUMMODEM)
			continue;
		if(procInfo.numCD < 0 || procInfo.numCD > SYSTEMNUMCD)
			continue;

		// Assign the process a number
	    procInfo.num = procNum++;

	    // Mark the process as not running
		procInfo.pid = 0;
		procInfo.status = PROCNOTRUNNING;

		// Check if we can add this process to the job list
	    if(!ProcManLinkNode(ListAppend(jobList,(&procInfo))))
	    {	
	    	// If we cannot, close the file and clean up
	        fclose(inputfile);
	        ListDelete(jobList);
	        return NULL;
	    }
	}

	fclose(inputfile);

	return jobList;
}