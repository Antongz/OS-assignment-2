/*
 * Memory.h
 *
 *  Created on: 29 Sep. 2017
 *      Author(s): Cyrus Villacampa 	 - a1709135
 *      		   Mohammad Idrees Rezai - a1687415
 */

#ifndef MEMSIM_MEMORY_H
#define MEMSIM_MEMORY_H

#include <vector>
#include "Frame.h"

class Memory {
private:
    std::vector<Frame> memory;
    std::vector<Frame>::iterator findHelper(Frame f);				// A helper function that returns true if "f" is in memory
public:
    Memory();
    bool find(Frame f);												// Finds the given argument in memory and returns true if found otherwise false
    Frame get(unsigned int pos);									// Returns the frame on the given position(throws out_of_bounds)
    void remove(Frame f);											// Removes the given argument in memory
    Frame pop();													// Remove and return the head of the memory
    void push(Frame f);												// Insert the given argument at the end/tail of the memory
    unsigned int numOfFrames();										// Returns the number of frames in memory
    void shiftRightReferenceBits(unsigned int numOfTimes);			// Shift the reference bits of all frames in memory to the right by "numOfTimes"
    void setReferenceBits(Frame f, unsigned int pattern);			// Sets the individual bits of frame f's reference bits according to the given pattern
    void updateHasBeenModified(Frame f);							// Updates the has been modified of this frame in memory in place
    bool isEmpty();													// Returns true if memory is empty otherwise false
    virtual ~Memory();
};

#endif //MEMSIM_MEMORY_H
