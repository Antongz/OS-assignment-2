/*
 * FIFO.cpp
 *
 *  Created on: 29 Sep. 2017
 *      Author(s): Cyrus Villacampa 	 - a1709135
 *      		   Mohammad Idrees Rezai - a1687415
 */

#include <iostream>
#include <math.h>
#include "FIFO.h"

FIFO::FIFO(
			bool outputMode,
			unsigned long pageSize,
			unsigned long pageFrames,
			std::queue<MemoryEvent> stackTrace) {
	this->outputMode = outputMode;
	this->pageSize = pageSize;
	this->pageFrames = pageFrames;
	this->stackTrace.swap(stackTrace);
	init();
	initOffset(pageSize);
}

void FIFO::init() {
	this->numOfEvents = this->stackTrace.size();
	this->numOfDiskReads = 0;
	this->numOfDiskWrites = 0;
	this->numOfPageFaults = 0;
	this->numOfPrefetchFaults = 0;
}

void FIFO::initOffset(unsigned long pageSize) {
	unsigned long tempPageSize = 0;
	unsigned int bits = 0;

	while (tempPageSize < pageSize) {
		bits += 1;
		tempPageSize = pow(2, bits);
	}

	this->offsetBits = bits;
}

void FIFO::start() {
	while (!stackTrace.empty()) {
		MemoryEvent memEvent = this->popTraceHead();
		Frame f = this->createFrame(memEvent);
		if (!memory.find(f)) {						// Check if the page is already in memory. Frame not yet in memory, need to load from disk
			if (this->outputMode) {
				std::cout << "MISS:\t page " << f.getPageNumber() << std::endl;
			}
			this->addFrame(f);
			this->numOfPageFaults += 1;
		} else {
			if (this->outputMode) {
				std::cout	 << "HIT:\t page "	<< f.getPageNumber() <<	std::endl;
			}
			if (memEvent.getOperation()) {
				memory.updateHasBeenModified(f);
			}
		}
	}
	this->printSummary();
}

void FIFO::addFrame(Frame f) {
	if (memory.numOfFrames() == this->pageFrames) {	// Memory is full. Evict a victim page.
		Frame evictedFrame = memory.pop();
		if (this->outputMode) {
			std::cout	 << "REPLACE: page "	<< evictedFrame.getPageNumber();
		}
		if (evictedFrame.getHasBeenModified()) {		// If the evicted page is dirty then write it on disk.
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

Frame FIFO::createFrame(MemoryEvent memEvent) {
	Frame newFrame;
	newFrame.setHasBeenModified(memEvent.getOperation());
	newFrame.setPageNumber(memEvent.getMemoryAddress() >> this->offsetBits);		// Shift the memory address(as an unsigned long long) to the right for "offsetBits" to extract the page number
	return newFrame;
}

void FIFO::printSummary() {
	std::cout <<		"events in trace:      "		<< 		this->numOfEvents 			<< std::endl;
    std::cout <<		"total disk reads:     "		<<		this->numOfDiskReads		<< std::endl;
    std::cout <<		"total disk writes:    "		<<		this->numOfDiskWrites		<< std::endl;
    std::cout <<		"page faults:          "		<<		this->numOfPageFaults		<< std::endl;
    std::cout <<		"prefetch faults:      "		<<		this->numOfPrefetchFaults	<< std::endl;
}

MemoryEvent FIFO::popTraceHead() {
	MemoryEvent retVal = stackTrace.front();
	stackTrace.pop();
	return retVal;
}

FIFO::~FIFO() {

}

