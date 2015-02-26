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
//	Proc.h
//=======================================
//	Header for Process execution
// 
//	This forms the child process the hostd
//	dispatcher dispatches
//
//	The process is under control of the hostd
//	dipatcher
//

#pragma once

#include "ProcMan.h"

void ProcFunc(ProcInfo* proc);