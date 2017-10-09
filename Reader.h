/*
 * Reader.h
 *
 *  Created on: 29 Sep. 2017
 *      Author(s): Cyrus Villacampa 	 - a1709135
 *      		   Mohammad Idrees Rezai - a1687415
 */

#ifndef MEMSIM_READER_H
#define MEMSIM_READER_H

#include <fstream>
#include <queue>
#include <string>
#include "MemoryEvent.h"

class Reader {
private:
    std::string
    extractProcessId(std::string str);            // Extract and return the string representation of the process id
    std::string
    extractMemoryAddress(std::string str);        // Extract and return the string representation of the memory address
    bool extractOperation(
            std::string str);                    // Extract the operation and return true if it is a write otherwise false
public:
    Reader();

    virtual ~Reader();

    std::queue<MemoryEvent> read(std::ifstream &in);
};

#endif //MEMSIM_READER_H
