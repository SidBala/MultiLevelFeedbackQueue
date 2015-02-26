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
//	Visualizer.h
//=======================================
//	Header for state visualizer
// 
//	Given the instance to a MMU/MLFQ, prints
//	out information on to console UI
//

#pragma once

#include "List.h"
#include "MLFQ.h"
#include "ProcMan.h"
#include "MMU.h"

typedef enum  
{
	BLACK,
	RED,
	GREEN,
	YELLOW,
	BLUE,
	MAGENTA,
	CYAN,
	WHITE
} VISCOL;

typedef enum  
{
	RESET,
	BRIGHT,
	DIM,
	UNDERLINE,
	BLINK,
	REVERSE,
	HIDDEN
} VISSTYLE;

void VisChangeColor(VISCOL colf, VISCOL colb, VISSTYLE style);
void VisResetColor(void);
void VisClearScreen(void);
void VisSetCursorPos(int x, int y);
void VisPrintState(void);