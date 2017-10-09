/*
 * WorkingSet.h
 *
 *   Created on: 1 Oct. 2017
 * 		 Author(s): Cyrus Villacampa 	 - a1709135
 *      		    Mohammad Idrees Rezai - a1687415
 */

#ifndef MEMSIM_WORKINGSET_H
#define MEMSIM_WORKINGSET_H

#include <vector>

class WorkingSet {
private:
    WorkingSet();
    std::vector<unsigned long> workingSet;					// The working set of a process
    unsigned int processId;									// The process id which this working set belongs to
    unsigned int size;										// The working set size
public:
    WorkingSet(unsigned int processId, unsigned int size);
    void add(unsigned long pageNumber);						// Adds the page number in the current working set
    unsigned long get(unsigned int pos);					// Returns the element at the given position
    unsigned int numOfElements();							// Returns the number of elements currently in the working set
    void updateSize(unsigned int newSize);					// Updates the size of the working set
    bool isEmpty();											// Test if the working set is empty
    unsigned int getProcessId();
    virtual ~WorkingSet();
};

#endif //MEMSIM_WORKINGSET_H
