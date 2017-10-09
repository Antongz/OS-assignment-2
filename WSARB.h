/*
 * WSARB.h
 *
 *  Created on: 29 Sep. 2017
 *      Author(s): Cyrus Villacampa 	 - a1709135
 *      		   Mohammad Idrees Rezai - a1687415
 *
 * This is class is an implementation of the WSARB page replacement algorithm.
 */

#ifndef MEMSIM_WSARB_H
#define MEMSIM_WSARB_H


#include <queue>
#include <vector>
#include "Memory.h"
#include "Frame.h"
#include "WorkingSet.h"
#include "MemoryEvent.h"

class WSARB {
private:
    WSARB();
    unsigned long numOfEvents;							// The total number of memory access in the trace
    unsigned long numOfDiskReads;						// The total number of disk reads
    unsigned long numOfDiskWrites;						// The total number of disk writes
    unsigned long numOfPageFaults;						// The total number of page faults
    unsigned int numOfPrefetchFaults;					// The total number of prefetch faults
    bool outputMode;									// False for quiet, True for debug
    unsigned long pageSize;								// Size of a page
    unsigned long pageFrames;							// Number of page frames
    unsigned int offsetBits;							// Number of offset bits
    unsigned int interval;								// The interval in page references for shifting the reference bits
    unsigned int wss;									// The working set size
    unsigned long numOfReferences;						// The number of times a page/frame is referenced. This is incremented everytime a page, regardless if that page is in memory or not, is referenced.
    Memory memory;										// The memory where the frames are stored
    std::queue<MemoryEvent> stackTrace;					// The trace
    std::vector<WorkingSet*> processWorkingSets;		// A vector that maintains the working sets of each process
    void initOffset(unsigned long pageSize);			// A function that determines the number of bits associated with
    // a certain page size and sets the "offset" variable with that number
    void init();										// Initializes the variables
    WorkingSet* prefetch(unsigned int processId);		// Loads the pages of the process id's working set into memory and return a pointer to the process' working set
    WorkingSet* findWorkingSet(unsigned int processId);	// Looks for and returns the pointer to working set of the given process id, returns 0 if not found
    void fetchPages(WorkingSet* ws);					// Fetch the pages in the working set and store it in memory
    MemoryEvent popTraceHead();							// Remove and return the head of memory
    Frame createFrame(MemoryEvent memEvent);			// Creates a frame
    void addFrame(Frame f);			// Adds a frame if not yet in memory, evicts a frame in memory and updates the correct variables if necessary
    Frame evictPage();									// Evicts and returns a page in memory by their reference bits
public:
    WSARB(
            bool outpudeMode,
            unsigned long pageSize,
            unsigned long pageFrames,
            unsigned int interval,
            unsigned int wss,
            std::queue<MemoryEvent> stackTrace);
    void start();										// Starts the memory simulator using WSARB as its page replacement algorithm
    void printSummary();								// Prints the summary in std::cout
    virtual ~WSARB();
};

#endif //MEMSIM_WSARB_H
