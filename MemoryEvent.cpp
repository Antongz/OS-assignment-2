/*
 * MemoryEvent.cpp
 *
 *  Created on: 29 Sep. 2017
 *      Author(s): Cyrus Villacampa 	 - a1709135
 *      		   Mohammad Idrees Rezai - a1687415
 */

#include "MemoryEvent.h"

MemoryEvent::MemoryEvent() {
    this->memoryAddress = 0;
    this->operation = false;
    this->processId = 0;
}

MemoryEvent::MemoryEvent(unsigned long long memoryAddress, bool operation, unsigned int processId) {
    this->memoryAddress = memoryAddress;
    this->operation = operation;
    this->processId = processId;
}

void MemoryEvent::setMemoryAddress(unsigned long long address) {
    this->memoryAddress = address;
}

void MemoryEvent::setOperation(bool op) {
    this->operation = op;
}

void MemoryEvent::setProcessId(unsigned int id) {
    this->processId = id;
}

unsigned long long MemoryEvent::getMemoryAddress() {
    return this->memoryAddress;
}

bool MemoryEvent::getOperation() {
    return this->operation;
}

unsigned int MemoryEvent::getProcessId() {
    return this->processId;
}

MemoryEvent::~MemoryEvent() {

}

