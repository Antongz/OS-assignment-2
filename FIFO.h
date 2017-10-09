/*
 * FIFO.h
 *
 *  Created on: 29 Sep. 2017
 *      Author(s): Cyrus Villacampa 	 - a1709135
 *      		   Mohammad Idrees Rezai - a1687415
 */

#ifndef MEMSIM_FIFO_H
#define MEMSIM_FIFO_H

#include <queue>
#include "Frame.h"
#include "Memory.h"
#include "MemoryEvent.h"

class FIFO {
private:
    FIFO();											// Default constructor unavailable
    unsigned long numOfEvents;						// The total number of memory access in the trace
    unsigned long numOfDiskReads;					// The total number of disk reads
    unsigned long numOfDiskWrites;					// The total number of disk writes
    unsigned long numOfPageFaults;					// The total number of page faults
    unsigned int numOfPrefetchFaults;				// The total number of prefetch faults
    bool outputMode;								// False for quiet, True for debug
    unsigned long pageSize;							// Size of a page
    unsigned long pageFrames;						// Number of page frames
    unsigned int offsetBits;						// Number of offset bits
    Memory memory;									// The memory where the frames are stored
    std::queue<MemoryEvent> stackTrace;				// The trace
    void initOffset(unsigned long pageSize);		// A function that determines the number of bits associated with
    // a certain page size and sets the "offset" variable with that number
    void init();									// Initializes the variables
    MemoryEvent popTraceHead();						// Remove and return the head of memory
    Frame createFrame(MemoryEvent memEvent);		// Creates a frame
    void addFrame(Frame f);							// Adds a frame if not yet in memory, evicts a frame in memory and updates variables if necessary
public:
    FIFO(
            bool outputMode,
            unsigned long pageSize,
            unsigned long pageFrames,
            std::queue<MemoryEvent> stackTrace);
    void start();									// Starts the memory simulator using FIFO as its page replacement algorithm
    void printSummary();							// Prints the summary in std::cout
    virtual ~FIFO();
};

#endif //MEMSIM_FIFO_H
