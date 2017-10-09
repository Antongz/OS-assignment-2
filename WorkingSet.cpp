/*
 * WorkingSet.cpp
 *
 *   Created on: 1 Oct. 2017
 * 		 Author(s): Cyrus Villacampa 	 - a1709135
 *      		    Mohammad Idrees Rezai - a1687415
 */

#include "WorkingSet.h"

WorkingSet::WorkingSet(unsigned int processId, unsigned int size) {
    this->size = size;
    this->processId = processId;
}

void WorkingSet::add(unsigned long pageNumber) {
    if (workingSet.size() == this->size) {
        workingSet.erase(workingSet.begin());
    }
    workingSet.push_back(pageNumber);
}

unsigned long WorkingSet::get(unsigned int pos) {
    if (pos >= workingSet.size()) {
        throw "Out of bounds.";
    }
    return workingSet.at(pos);
}

unsigned int WorkingSet::numOfElements() {
    return workingSet.size();
}

void WorkingSet::updateSize(unsigned int newSize) {
    if (newSize == 0) {
        throw "Size cannot be zero.";
    }
    this->size = newSize;
}

bool WorkingSet::isEmpty() {
    return workingSet.empty();
}

unsigned int WorkingSet::getProcessId() {
    return this->processId;
}

WorkingSet::~WorkingSet() {

}