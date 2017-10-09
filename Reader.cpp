/*
 * Reader.cpp
 *
 *  Created on: 29 Sep. 2017
 *      Author(s): Cyrus Villacampa 	 - a1709135
 *      		   Mohammad Idrees Rezai - a1687415
 */

#include <string>
#include <iostream>
#include <ctype.h>
#include "Reader.h"

Reader::Reader() {

}

std::queue<MemoryEvent> Reader::read(std::ifstream& in) {
    std::queue<MemoryEvent> stackTrace;
    std::string line = "";
    unsigned int processId = 0;
    while(!getline(in, line).eof()) {
        if (line.find_first_of("#") != std::string::npos) {			// A context switch. Read and update process id.
            processId = stoul(this->extractProcessId(line));
        } else {													// A memory event. Create a memory event and push it on stack.
            unsigned long long decMemAddress = stoull(this->extractMemoryAddress(line), nullptr, 0);
            MemoryEvent memEvent (decMemAddress, extractOperation(line), processId);
            stackTrace.push(memEvent);
        }
    }
    return stackTrace;
}

std::string Reader::extractProcessId(std::string str) {
    size_t found = str.find_first_of("0123456789");
    std::string procId = "";
    while (found != std::string::npos) {
        procId.append(str, found, 1);
        if (isdigit(str[found+1])) {
            found += 1;
        } else {
            found = std::string::npos;
        }
    }

    if (procId == "") {
        std::cout << "[ ERROR ] Missing process id" << std::endl;
        exit(EXIT_FAILURE);
    }

    return procId;
}

std::string Reader::extractMemoryAddress(std::string str) {
    size_t found = str.find_first_of("0123456789abcdefABCDEF");
    std::string memAddress = "0x";
    while (found != std::string::npos) {
        memAddress.append(str, found, 1);
        found = str.find_first_of("0123456789abcdefABCDEF", found+1);
    }

    if (memAddress.compare("0x") == 0) {
        std::cout << "[ ERROR ] Missing memory address" << std::endl;
        exit(EXIT_FAILURE);
    }

    return memAddress;
}

bool Reader::extractOperation(std::string str) {
    size_t found;
    found = str.find_first_of("Ww");
    if (found != std::string::npos) {
        return true;
    } else {
        return false;
    }
}

Reader::~Reader() = default;

