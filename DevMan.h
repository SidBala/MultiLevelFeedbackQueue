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
//	Devman.h
//=======================================
//	Header for devicemanager module
// 
//	Handles requests for usage of devices 
//  and peripherals.
//

#pragma once

#include "List.h"
#include "MMU.h"
#include "ProcMan.h"

typedef struct _DeviceState DeviceState;

typedef enum  
{
	DEVMANSUCCESS,
    DEVMANAVAILABLE,
    DEVMANUNAVAILABLE,
    DEVMANALLOCATED,
    DEVMANRELEASEFAIL,
} DEVMANERR;

struct _DeviceState
{
	int cd;
	int scan;
	int print;
	int modem;
};

DEVMANERR DevManInit();
DEVMANERR DevManCheckAllocate(ProcInfo* procInfo);
DEVMANERR DevManAllocate(ProcInfo* procInfo);
DEVMANERR DevManRelease(ProcInfo* procInfo);
DeviceState* DevManGetDeviceState(void);