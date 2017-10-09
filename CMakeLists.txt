cmake_minimum_required(VERSION 3.8)
project(memsim)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp ARB.cpp ARB.h Memory.cpp Memory.h Frame.cpp Frame.h MemoryEvent.cpp MemoryEvent.h Reader.cpp Reader.h FIFO.cpp FIFO.h WSARB.cpp WSARB.h WorkingSet.cpp WorkingSet.h)
add_executable(memsim ${SOURCE_FILES})