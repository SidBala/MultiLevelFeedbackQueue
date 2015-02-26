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
//	Proc.c
//=======================================
//	Process execution functions
// 
//	This forms the child process that the
//  hostd dispatcher dispatches
//
//	The process is under control of the hostd
//	dipatcher and requires it to issue suspend
//  resume and terminate signals
//
//	If no terminate signal is received, the
//  process automatically terminates after
//  a certain timeout period
//

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#include "Proc.h"
#include "SystemDefs.h"
#include "Visualizer.h"

int procNum;
int time;
int totalTime;

void _SuspendFunc(int sig)
// Handler for SIGTSTP
{
	// Use yellow to display a suspension message
	VisChangeColor(YELLOW, BLACK, BRIGHT);
	printf("\nProcess Suspending #%d", procNum);
	VisResetColor();

	// Flush the screen buffer to display the message now
	fflush(stdout);

	kill(getpid(),SIGSTOP);
}

void _ResumeFunc(int sig)
// Handler for SIGCONT
{
	// Use blue to display a resume message
	VisChangeColor(BLUE, BLACK, BRIGHT);
	printf("\nProcess Resuming #%d", procNum);
	VisResetColor();

	fflush(stdout);
}

void _TerminateFunc(int sig)
// Handler for SIGINT
{	
	// Use red to display termination message
	VisChangeColor(RED,BLACK,BRIGHT);
	printf("\nProcess Terminating #%d",procNum);
	VisResetColor();

	fflush(stdout);

	// Exit the process
	exit(sig);
}


void ProcFunc(ProcInfo* proc)
// This is the main entry point for the child process
// The process prints a regular status message every time quantum
// and updates an internal CPUtime counter.
// If the counter exceeds the timeout, it exits the process
{
	procNum = proc->num;
	totalTime = proc->cpuTime;

	// Setup handlers for signals
	signal(SIGTSTP, _SuspendFunc);
	signal(SIGINT, _TerminateFunc);
	signal(SIGCONT, _ResumeFunc);
	
	// Use blue to display a message indicating the new process has started
	VisChangeColor(BLUE,BLACK,BRIGHT);
	printf("\nProcess Started #%d CPUTime:%d", procNum, totalTime);
	VisResetColor();
	
	// Print a regular status message every time quantum indicating the process is running
	do{
		time++;

		// Use green for the message
		VisChangeColor(GREEN,BLACK,BRIGHT);
		printf("\nProcess Running #%d CPU:%d/%d", procNum, time, totalTime);
		VisResetColor();

		fflush(stdout);

		sleep(SYSTEMTIMEQUANT);

	}while(time < SYSTEMPROCTIMEOUT);

	// If the process timeout has exceeded, kill the process.
	// Ideally this should never happen. And it doesn't.
	printf("\nProcess Terminating due to timeout #%d",procNum);

	exit(EXIT_SUCCESS);
}