/*
 * Frame.h
 *
 *  Created on: 29 Sep. 2017
 *      Author(s): Cyrus Villacampa 	 - a1709135
 *      		   Mohammad Idrees Rezai - a1687415
 */

#ifndef MEMSIM_FRAME_H
#define MEMSIM_FRAME_H

class Frame {
private:
    unsigned long pageNumber;			// The page number(i.e. n-m bits of the memory address where m is the size of each page)
    bool hasBeenModified;				// True if page has been written/modified, false otherwise
    unsigned char referenceBits;		// The reference bits for ARB and WSARB page replacement algorithm
public:
    Frame();
    Frame(unsigned long pageNumber, bool hasBeenModified);
    void setPageNumber(unsigned long val);
    unsigned long getPageNumber();
    void setHasBeenModified(bool val);
    bool getHasBeenModified();
    void setReferenceBits(unsigned char val);
    unsigned char getReferenceBits();
    virtual ~Frame();
};

#endif //MEMSIM_FRAME_H
