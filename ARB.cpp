/*
 * ARB.cpp
 *
 *  Created on: 29 Sep. 2017
 *      Author(s): Cyrus Villacampa 	 - a1709135
 *      		   Mohammad Idrees Rezai - a1687415
 */

#include <iostream>
#include <math.h>
#include "ARB.h"

ARB::ARB(
        bool outputMode,
        unsigned long pageSize,
        unsigned long pageFrames,
        unsigned int interval,
        std::queue<MemoryEvent> stackTrace) {
    this->outputMode = outputMode;
    this->pageSize = pageSize;
    this->pageFrames = pageFrames;
    this->interval = interval;
    this->stackTrace.swap(stackTrace);
    init();
    initOffset(pageSize);
}

void ARB::init() {
    this->numOfEvents = this->stackTrace.size();
    this->numOfDiskReads = 0;
    this->numOfDiskWrites = 0;
    this->numOfPageFaults = 0;
    this->numOfPrefetchFaults = 0;
    this->numOfReferences = 0;
}

void ARB::initOffset(unsigned long pageSize) {
    unsigned long tempPageSize = 0;
    unsigned int bits = 0;

    while (tempPageSize < pageSize) {
        bits += 1;
        tempPageSize = pow(2, bits);
    }

    this->offsetBits = bits;
}

void ARB::start() {
    while (!stackTrace.empty()) {
        numOfReferences += 1;
        MemoryEvent memEvent = this->popTraceHead();
        Frame f = this->createFrame(memEvent);
        if (!memory.find(f)) {							// Page not found in memory
            if (this->outputMode) {
                std::cout << "MISS:\t page " << f.getPageNumber() << std::endl;
            }
            this->addFrame(f);
            this->numOfPageFaults += 1;
        } else {										// Page already in memory
            if (this->outputMode) {
                std::cout << "HIT:\t page "	<< f.getPageNumber() <<	std::endl;
            }
            if (memEvent.getOperation()) {				// If its a write operation then update the hasBeenModified field
                memory.updateHasBeenModified(f);
            }
        }
        /////////////////////////////////////////////////////
        // Update the reference bits first before shifting //
        /////////////////////////////////////////////////////
        memory.setReferenceBits(f, 128);				// Set the most significant bit of the reference bits of the page that was referenced
        if (numOfReferences % interval == 0) {			// Shift the reference bits of all pages at an interval
            memory.shiftRightReferenceBits(1);			// Shift the reference bits of all pages to the right by one
        }
    }

    this->printSummary();
}

void ARB::addFrame(Frame f) {
    if (memory.numOfFrames() == this->pageFrames) {		// Memory is full. Evict a victim page.
        Frame evictedPage = this->evictPage();
        if (this->outputMode) {
            std::cout << "REPLACE: page " << evictedPage.getPageNumber();
        }
        if (evictedPage.getHasBeenModified()) {			// If the evicted page is dirty then write it on disk.
            this->numOfDiskWrites += 1;
            if (this->outputMode) {
                std::cout << " (DIRTY)" << std::endl;
            }
        } else {
            if (this->outputMode) {
                std::cout << std::endl;
            }
        }
    }
    memory.push(f);
    this->numOfDiskReads += 1;
}

Frame ARB::evictPage() {
    Frame evictedPage;
    try {
        evictedPage = memory.get(0);
    } catch (const char* msg) {
        std::cerr << msg << std::endl;
    }

    for (size_t i = 1; i < memory.numOfFrames(); i += 1) {							// Loop through the memory to check which page has the smallest reference bits
        try {
            Frame f = memory.get(i);
            if (((unsigned int)f.getReferenceBits()) < ((unsigned int)evictedPage.getReferenceBits())) {
                evictedPage = f;
            }
        } catch (const char* msg) {
            std::cerr << msg << std::endl;
        }
    }
    memory.remove(evictedPage);
    return evictedPage;
}

Frame ARB::createFrame(MemoryEvent memEvent) {
    Frame newFrame;
    newFrame.setHasBeenModified(memEvent.getOperation());
    newFrame.setPageNumber(memEvent.getMemoryAddress() >> this->offsetBits);		// Shift the memory address(as an unsigned long long) to the right for "offsetBits" to extract the page number
    return newFrame;
}

MemoryEvent ARB::popTraceHead() {
    MemoryEvent retVal = stackTrace.front();
    stackTrace.pop();
    return retVal;
}

void ARB::printSummary() {
    std::cout <<		"events in trace:      "		<< 		this->numOfEvents 			<< std::endl;
    std::cout <<		"total disk reads:     "		<<		this->numOfDiskReads		<< std::endl;
    std::cout <<		"total disk writes:    "		<<		this->numOfDiskWrites		<< std::endl;
    std::cout <<		"page faults:          "		<<		this->numOfPageFaults		<< std::endl;
    std::cout <<		"prefetch faults:      "		<<		this->numOfPrefetchFaults	<< std::endl;
}

ARB::~ARB() {

}

