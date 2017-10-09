/*
 * ARB.h
 *
 *  Created on: 29 Sep. 2017
 *      Author(s): Cyrus Villacampa 	 - a1709135
 *      		   Mohammad Idrees Rezai - a1687415
 */

#ifndef MEMSIM_ARB_H
#define MEMSIM_ARB_H

#include <queue>
#include "Frame.h"
#include "Memory.h"
#include "MemoryEvent.h"

class ARB {
private:
    ARB();
    unsigned long numOfEvents;						// The total number of memory access in the trace
    unsigned long numOfDiskReads;					// The total number of disk reads
    unsigned long numOfDiskWrites;					// The total number of disk writes
    unsigned long numOfPageFaults;					// The total number of page faults
    unsigned int numOfPrefetchFaults;				// The total number of prefetch faults
    bool outputMode;								// False for quiet, True for debug
    unsigned long pageSize;							// Size of a page
    unsigned long pageFrames;						// Number of page frames
    unsigned int offsetBits;						// Number of offset bits
    unsigned int interval;							// The interval in page references for shifting the reference bits
    unsigned long numOfReferences;					// The number of times a page/frame is referenced. This is incremented everytime a page, regardless if that page is in memory or not, is referenced.
    Memory memory;									// The memory where the frames are stored
    std::queue<MemoryEvent> stackTrace;				// The trace
    void initOffset(unsigned long pageSize);		// A function that determines the number of bits associated with
    // a certain page size and sets the "offset" variable with that number
    void init();									// Initializes the variables
    MemoryEvent popTraceHead();						// Remove and return the head of memory
    Frame createFrame(MemoryEvent memEvent);		// Creates a frame
    void addFrame(Frame f);							// Adds a frame if not yet in memory, evicts a frame in memory and updates variables if necessary
    Frame evictPage();								// Evicts and returns a page in memory by their reference bits
public:
    ARB(
            bool outputMode,
            unsigned long pageSize,
            unsigned long pageFrames,
            unsigned int interval,
            std::queue<MemoryEvent> stackTrace);
    void start();									// Starts the memory simulator using ARB as its page replacement algorithm
    void printSummary();							// Prints the summary in std::cout
    virtual ~ARB();
};

#endif //MEMSIM_ARB_H
