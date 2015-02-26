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
//	SystemDefs.h
//=======================================
//	Header for the System Definition
// 
//	Specify the system parameters here.
//	Specify number of devices, memory size,
//  dispatcher queue levels and time quantum
//	size and process timeout
//
//	All constant parameters in the whole program
//  are defined here. Hence the program will run 
//  even with completely new parameters
//
//	Eg: a 6 level feedback queue can easily be
//      simulated by changing the number here
//


#pragma once

#define SYSTEMMEMSIZE 1024		// Total memory available
#define SYSTEMRTRESERVEMEM 64	// Reserved memory for realtime processes
#define SYSTEMNUMCD 2  			// Number of CD Drives
#define SYSTEMNUMSCAN 1 		// Number of scanners
#define SYSTEMNUMPRINT 2 		// Number of printers
#define SYSTEMNUMMODEM 1 		// Number of modems

#define SYSTEMMLFQLEVELS 4		// Number of levels in the MLFQ.
								// Level 0 is always the realtime queue
#define SYSTEMTIMEQUANT 1 		// Time quantum size
#define SYSTEMPROCTIMEOUT 20 	// Process timeout
