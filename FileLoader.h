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
//	FileLoader.h
//=======================================
//	Header for FileLoader module
// 
//	Loads the job lists from a specified 
//	file
//

#pragma once

#include "ProcMan.h"
#include "List.h"

typedef enum  
{
    FILESUCCESS,
	FILEDOESNOTEXIST,
    FILEINVALIDARG

} FILEERR;

FILEERR FileExists(char* filename);
List* FileLoadJobs(char* filename);
