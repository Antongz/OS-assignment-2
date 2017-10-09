/*
 * Frame.cpp
 *
 *  Created on: 29 Sep. 2017
 *      Author(s): Cyrus Villacampa 	 - a1709135
 *      		   Mohammad Idrees Rezai - a1687415
 */

#include "Frame.h"

Frame::Frame() {
    this->pageNumber = 0;
    this->hasBeenModified = false;
    this->referenceBits = ('0' & 0);			// Set history bits and reference bit to 0b000000000
}

Frame::Frame(unsigned long pageNumber, bool hasBeenModified) {
    this->pageNumber = pageNumber;
    this->hasBeenModified = hasBeenModified;
    this->referenceBits = ('0' & 0);			// Set history bits and reference bit to 0b000000000
}

void Frame::setPageNumber(unsigned long val) {
    this->pageNumber = val;
}

void Frame::setHasBeenModified(bool val) {
    this->hasBeenModified = val;
}

void Frame::setReferenceBits(unsigned char val) {
    this->referenceBits = val;
}

unsigned long Frame::getPageNumber() {
    return this->pageNumber;
}

bool Frame::getHasBeenModified() {
    return this->hasBeenModified;
}

unsigned char Frame::getReferenceBits() {
    return this->referenceBits;
}

Frame::~Frame() {

}

