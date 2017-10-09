/*
 * Memory.cpp
 *
 *  Created on: 29 Sep. 2017
 *      Author(s): Cyrus Villacampa 	 - a1709135
 *      		   Mohammad Idrees Rezai - a1687415
 */

#include "Memory.h"

using namespace std;

Memory::Memory() {

}

bool Memory::find(Frame f) {
    vector<Frame>::iterator it = findHelper(f);
    if (it >= memory.end()) {
        return false;
    }
    return true;
}

Frame Memory::get(unsigned int pos) {
    if (pos >= memory.size()) {
        throw "Out of bounds.";
    }
    return memory.at(pos);
}

void Memory::remove(Frame f) {
    vector<Frame>::iterator it = findHelper(f);
    if (it >= memory.begin() && it < memory.end()) {
        memory.erase(it);
    }
}

void Memory::updateHasBeenModified(Frame f) {
    vector<Frame>::iterator foundFrame = findHelper(f);
    if (foundFrame < memory.end()) {
        foundFrame->setHasBeenModified(f.getHasBeenModified());
    }
}

Frame Memory::pop() {
    Frame retVal;
    retVal = memory.front();
    remove(retVal);
    return retVal;
}

void Memory::push(Frame f) {
    memory.push_back(f);
}

unsigned int Memory::numOfFrames() {
    return memory.size();
}

void Memory::shiftRightReferenceBits(unsigned int numOfTimes) {
    for (vector<Frame>::iterator it = memory.begin(); it != memory.end(); it += 1) {
        unsigned char newReferenceBits = it->getReferenceBits();
        newReferenceBits >>= numOfTimes;
        it->setReferenceBits(newReferenceBits);
    }
}

void Memory::setReferenceBits(Frame f, unsigned int pattern) {
    vector<Frame>::iterator foundFrame = findHelper(f);
    unsigned char newReferenceBits;
    if (foundFrame < memory.end()) {
        newReferenceBits = foundFrame->getReferenceBits();
        newReferenceBits |= pattern;
        foundFrame->setReferenceBits(newReferenceBits);
    }
}

bool Memory::isEmpty() {
    return memory.empty();
}

vector<Frame>::iterator Memory::findHelper(Frame f) {
    vector<Frame>::iterator it = memory.end();

    if (!memory.empty()) {
        for (it = memory.begin(); it != memory.end(); it += 1) {
            if ((*it).getPageNumber() == f.getPageNumber()) {
                return it;
            }
        }
    }

    return it;
}

Memory::~Memory() {

}

