/*
 * main.cpp
 *
 * 	Created on: 29 Sep 2017
 * 		Author(s): Cyrus Villacampa 	 - a1709135
 *      		   Mohammad Idrees Rezai - a1687415
 */

#include <iostream>
#include <cstring>
#include <stdlib.h>
#include <queue>
#include <fstream>
#include "FIFO.h"
#include "ARB.h"
#include "WSARB.h"
#include "Reader.h"

void checkNumArgs(int, int);
bool readOutputMode(char *mode);
void openFile(char *filename, std::ifstream& in);
unsigned long parseNumber(char *num);
char readReplacementAlgo(char *ra);
void runSimulator(
        std::queue<MemoryEvent> stackTrace,
        bool outputMode,
        unsigned long pageSize,
        unsigned long pageFrames,
        char replacementAlgo,
        int numOfArguments,
        char *argv[]);
char* convertToLower(char *str);

int main(int argc, char *argv[]) {
    Reader reader;
    std::ifstream in;
    std::queue<MemoryEvent> stackTrace;
    bool outputMode = false;				// False for quiet, True for debug
    unsigned long pageSize = 0;
    unsigned long pageFrames = 0;
    char replacementAlgorithm = ' ';		// f - FIFO, a - ARB, w - WSARB

    checkNumArgs(argc, 5);
    openFile(argv[1], in);
    outputMode = readOutputMode(convertToLower(argv[2]));
    pageSize = parseNumber(argv[3]);
    pageFrames = parseNumber(argv[4]);
    replacementAlgorithm = readReplacementAlgo(argv[5]);
    stackTrace = reader.read(in);
    runSimulator(
            stackTrace,
            outputMode,
            pageSize,
            pageFrames,
            replacementAlgorithm,
            argc,
            argv);
}

void checkNumArgs(int numOfArgs, int expected);

void openFile(char *filename, std::ifstream& in) {
    // open file
    in.open(filename);
    // check if file has been successfully opened
    if (!in.is_open()) {
        std::cout << "File does not exist or invalid file name"	<< std::endl;
        exit(EXIT_FAILURE);
    }
}

bool readOutputMode(char *mode) {
    // check the chosen output mode
    if (strcmp(mode, "debug") != 0 || strcmp(mode, "quiet") != 0) {
        if (strcmp(mode, "debug") == 0) {
            return true;
        }
    } else {
        std::cout << "Invalid output mode(quiet/debug)"	<< std::endl;
        exit(EXIT_FAILURE);
    }

    return false;
}

char readReplacementAlgo(char *ra) {
    char c = ' ';

    if (strcmp(ra, "fifo") == 0) {
        c = 'f';
    }
    if (strcmp(ra, "arb") == 0) {
        c = 'a';
    }
    if (strcmp(ra, "wsarb") == 0) {
        c = 'w';
    }
    if (c == ' ') {
        std::cout << "Invalid replacement algorithm(fifo/arb/wsarb)" << std::endl;
        exit(EXIT_FAILURE);
    }

    return c;
}

unsigned long parseNumber(char *num) {
    unsigned long ul = strtoul(num, NULL, 0);

    if (ul == 0) {
        std::cout << "The page size or number of page frames cannot be zero or negative" << std::endl;
        exit(EXIT_FAILURE);
    }

    return ul;
}

void runSimulator(
        std::queue<MemoryEvent> stackTrace,
        bool outputMode,
        unsigned long pageSize,
        unsigned long pageFrames,
        char replacementAlgo,
        int numOfArguments,
        char *argv[]) {
    unsigned long interval;
    unsigned long workingSetSize;
    switch (replacementAlgo) {
        case 'f': {
            FIFO fifo(outputMode, pageSize, pageFrames, stackTrace);
            fifo.start();
            break;
        }
        case 'a': {
            checkNumArgs(numOfArguments, 6);
            interval = parseNumber(argv[6]);
            ARB arb(outputMode, pageSize, pageFrames, interval, stackTrace);
            arb.start();
            break;
        }
        case 'w': {
            checkNumArgs(numOfArguments, 7);
            interval = parseNumber(argv[6]);
            workingSetSize = parseNumber(argv[7]);
            WSARB wsarb(outputMode, pageSize, pageFrames, interval, workingSetSize, stackTrace);
            wsarb.start();
            break;
        }
        default:
            break;
    }
}

char* convertToLower(char *str) {
    char *retStr = str;
    int i = 0;

    while (str[i]) {
        retStr[i]=str[i];
        i++;
    }

    return retStr;
}

void checkNumArgs(int numOfArgs, int expected) {
    if (numOfArgs < expected) {
        std::cout 	<< "[ USAGE ]: ./memsim "								// argv[0]
                     << "(trace file filename) "								// argv[1]
                     << "(output mode(quiet/debug)) "						// argv[2]
                     << "(page/frame size(in bytes)) "						// argv[3]
                     << "(number of page frames) "							// argv[4]
                     << "(page replacement algorithm(fifo/arb/wsarb)) "		// argv[5]
                     << "[interval] "										// argv[6]
                     << "[size of working set window]"						// argv[7]
                     << std::endl;
        exit(EXIT_FAILURE);
    }
}
