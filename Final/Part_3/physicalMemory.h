/*
    CSE 312
    Operating Systems
    Final Project
    physicalMemory.h
    Omer CEVIK
    161044004
*/

#ifndef PHYSICAL_MEMORY_H
#define PHYSICAL_MEMORY_H

#include <iostream>
#include <cmath>

using namespace std;

class PhysicalMemory
{
public:
    PhysicalMemory(){}
    ~PhysicalMemory();
    PhysicalMemory(int frameSize, int numPhysical);

    int getPhysicalSize() { return this->numPhysical; }
    int getPhysicalMemorySize() { return this->physicalMemorySize; }

    int getFrameSize() { return this->frameSize; }
    int** getPages() { return this->pages; }

    void setValidAt(int index, bool v) { this->valids[index] = v; }
    bool getValidAt(int index) { return this->valids[index]; }

    int getValidPageIndex();

private:
    int** pages = NULL;
    bool* valids = NULL;

    int frameSize = -1;
    int numPhysical;
    int physicalMemorySize;
    int physicalPageCounter = 0;
};

#endif