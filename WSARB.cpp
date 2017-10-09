/*
 * WSARB.cpp
 *
 *  Created on: 29 Sep. 2017
 *      Author(s): Cyrus Villacampa - a1709135
 *      		   Mohammad Idrees Rezai - a1687415
 */

#include <iostream>
#include <cmath>
#include "WSARB.h"

WSARB::WSARB(
        bool outputMode,
        unsigned long pageSize,
        unsigned long pageFrames,
        unsigned int interval,
        unsigned int wss,
        std::queue<MemoryEvent> stackTrace) {
    this->outputMode = outputMode;
    this->pageSize = pageSize;
    this->pageFrames = pageFrames;
    this->interval = interval;
    this->wss = wss;
    this->stackTrace.swap(stackTrace);
    init();
    initOffset(pageSize);
}

void WSARB::init() {
    this->numOfEvents = this->stackTrace.size();
    this->numOfDiskReads = 0;
    this->numOfDiskWrites = 0;
    this->numOfPageFaults = 0;
    this->numOfPrefetchFaults = 0;
    this->numOfReferences = 0;
}

void WSARB::initOffset(unsigned long pageSize) {
    unsigned long tempPageSize = 0;
    unsigned int bits = 0;

    while (tempPageSize < pageSize) {
        bits += 1;
        tempPageSize = pow(2, bits);
    }

    this->offsetBits = bits;
}

void WSARB::start() {
    unsigned int prevProcessId = stackTrace.front().getProcessId();				// Grab the process id of the first process in the trace
    WorkingSet* currentWorkingSet = new WorkingSet(prevProcessId, this->wss);	// Create working set of the first process in the trace
    this->processWorkingSets.push_back(currentWorkingSet);						// Store it in the maintained working sets
    while (!stackTrace.empty()) {
        numOfReferences += 1;
        MemoryEvent memEvent = this->popTraceHead();
        if (prevProcessId != memEvent.getProcessId()) {							// There is a context switch
            prevProcessId = memEvent.getProcessId();							// Update the previous process id
            currentWorkingSet = this->prefetch(prevProcessId);
        }
        Frame f = this->createFrame(memEvent);
        currentWorkingSet->add(f.getPageNumber());
        if (!memory.find(f)) {													// Page not found in memory
            if (this->outputMode) {
                std::cout << "MISS:\t page " << f.getPageNumber() << std::endl;
            }
            this->addFrame(f);
            this->numOfPageFaults += 1;
        } else {																// Page already in memory
            if (this->outputMode) {
                std::cout << "HIT:\t page "	<< f.getPageNumber() <<	std::endl;
            }
            if (memEvent.getOperation()) {										// If its a write operation then update the hasBeenModified field
                memory.updateHasBeenModified(f);
            }
        }
        /////////////////////////////////////////////////////
        // Update the reference bits first before shifting //
        /////////////////////////////////////////////////////
        memory.setReferenceBits(f, 128);										// Set the most significant bit of the reference bits of the page that was referenced
        if (numOfReferences % interval == 0) {									// Shift the reference bits of all pages at an interval
            memory.shiftRightReferenceBits(1);									// Shift the reference bits of all pages to the right by one
        }
    }

    this->printSummary();
}

void WSARB::addFrame(Frame f) {
    if (memory.numOfFrames() == this->pageFrames) {		// Memory is full. Evict a victim page.
        Frame evictedPage = this->evictPage();
        if (this->outputMode) {
            std::cout	 << "REPLACE: page "	<< evictedPage.getPageNumber();
        }
        if (evictedPage.getHasBeenModified()) {			// If the evicted page is dirty then write it on disk.
            this->numOfDiskWrites += 1;
            if (this->outputMode) {
                std::cout	 << " (DIRTY)" << std::endl;
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

WorkingSet* WSARB::prefetch(unsigned int processId) {
    WorkingSet* currentWorkingSet = this->findWorkingSet(processId);
    if (currentWorkingSet == 0) {													// No working set found for this process therefore create a working set for this process
        currentWorkingSet = new WorkingSet(processId, this->wss);
        this->processWorkingSets.push_back(currentWorkingSet);
    } else {
        this->fetchPages(currentWorkingSet);
    }
    return currentWorkingSet;
}

WorkingSet* WSARB::findWorkingSet(unsigned int processId) {
    WorkingSet* currentWorkingSet = 0;
    for (std::vector<WorkingSet*>::iterator it = processWorkingSets.begin(); it != processWorkingSets.end(); ++it) {
        if ((*it)->getProcessId() == processId) {
            currentWorkingSet = (*it);
            break;
        }
    }
    return currentWorkingSet;
}

void WSARB::fetchPages(WorkingSet* ws) {
    unsigned int pos = 0;
    while (pos < ws->numOfElements()) {
        unsigned long pageNum = ws->get(pos);
        Frame f;
        f.setPageNumber(pageNum);
        if (!memory.find(f)) {
            this->addFrame(f);
            this->numOfPrefetchFaults += 1;
        }
        memory.setReferenceBits(f, 128);
        pos += 1;
    }
}

Frame WSARB::evictPage() {
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

Frame WSARB::createFrame(MemoryEvent memEvent) {
    Frame newFrame;
    newFrame.setHasBeenModified(memEvent.getOperation());
    newFrame.setPageNumber(memEvent.getMemoryAddress() >> this->offsetBits);		// Shift the memory address(as an unsigned long long) to the right for "offsetBits" to extract the page number
    return newFrame;
}

MemoryEvent WSARB::popTraceHead() {
    MemoryEvent retVal = stackTrace.front();
    stackTrace.pop();
    return retVal;
}

void WSARB::printSummary() {
    std::cout <<		"events in trace:      "		<< 		this->numOfEvents 			<< std::endl;
    std::cout <<		"total disk reads:     "		<<		this->numOfDiskReads		<< std::endl;
    std::cout <<		"total disk writes:    "		<<		this->numOfDiskWrites		<< std::endl;
    std::cout <<		"page faults:          "		<<		this->numOfPageFaults		<< std::endl;
    std::cout <<		"prefetch faults:      "		<<		this->numOfPrefetchFaults	<< std::endl;
}

WSARB::~WSARB() {
    while (!processWorkingSets.empty()) {
        // Release each allocated memory in the working set
        delete processWorkingSets.at(0);
        processWorkingSets.erase(processWorkingSets.begin());
    }
}

