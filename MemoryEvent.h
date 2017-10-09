/*
 * MemoryEvent.h
 *
 *  Created on: 29 Sep. 2017
 *      Author(s): Cyrus Villacampa 	 - a1709135
 *      		   Mohammad Idrees Rezai - a1687415
 */

#ifndef MEMSIM_MEMORYEVENT_H
#define MEMSIM_MEMORYEVENT_H

class MemoryEvent {
private:
    unsigned int processId;				// The process id of a process
    unsigned long long memoryAddress;	// The memory address to be accessed
    bool operation;						// The operation to be done on that address(True for write and False for read)
public:
    MemoryEvent();
    MemoryEvent(unsigned long long memoryAddress, bool operation, unsigned int processId);
    void setMemoryAddress(unsigned long long address);
    void setOperation(bool op);
    void setProcessId(unsigned int id);
    unsigned long long getMemoryAddress();
    bool getOperation();
    unsigned int getProcessId();
    virtual ~MemoryEvent();
};

#endif //MEMSIM_MEMORYEVENT_H
