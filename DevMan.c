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
//	DevMan.c
//=======================================
//	Device manager module
// 
//	Handles requests for usage of devices 
//  and peripherals.
//  
// 	Keeps a running count of free devices
//  and assigns devices to processes.
//  
//  Frees devices if a process terminates
//

#include "SystemDefs.h"
#include "ProcMan.h"
#include "DevMan.h"

int DevManisInit;

DeviceState devices;

DEVMANERR DevManInit()
// Initialize the device manager
{
	devices.cd = SYSTEMNUMCD;
	devices.scan = SYSTEMNUMSCAN;
	devices.print = SYSTEMNUMPRINT;
	devices.modem = SYSTEMNUMMODEM;
	DevManisInit = 1;

	return DEVMANSUCCESS;
}

DEVMANERR DevManCheckAllocate(ProcInfo* proc)
// Check if a certain set of devices can be allocated to a process
{
	if(!DevManisInit)
		DevManInit();

	if(devices.cd < proc->numCD)
		return DEVMANUNAVAILABLE;

	if(devices.scan < proc->numScan)
		return DEVMANUNAVAILABLE;

	if(devices.print < proc->numPrint)
		return DEVMANUNAVAILABLE;

	if(devices.modem < proc-> numModem)
		return DEVMANUNAVAILABLE;

	return DEVMANAVAILABLE;
}

DEVMANERR DevManAllocate(ProcInfo* proc)
// Allocate all the devices that are required by a process
{
	if(!DevManisInit)
		DevManInit();

	if(DevManCheckAllocate(proc) == DEVMANAVAILABLE)
	{
		devices.cd -= proc->numCD;
		devices.scan -= proc->numScan;
		devices.print -= proc->numPrint;
		devices.modem -= proc->numModem;

		return DEVMANALLOCATED;
	}
	else 
		return DEVMANUNAVAILABLE;
}

DEVMANERR DevManRelease(ProcInfo* proc)
// Release all the devices that has been allocated to a process
{
	if(proc->devBlock == DEVMANALLOCATED)
	{
		devices.cd += proc->numCD;
		devices.scan += proc->numScan;
		devices.print += proc->numPrint;
		devices.modem += proc->numModem;		

		proc->devBlock = 0;

		return DEVMANSUCCESS;
	}
	else 
		return DEVMANRELEASEFAIL;
}

DeviceState* DevManGetDeviceState(void)
// Return the device state
// Used to print the device state
{
	return &devices;
}