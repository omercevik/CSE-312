/*
    CSE 312
    Operating Systems
    Final Project
    physicalMemory.cpp
    Omer CEVIK
    161044004
*/

#include "physicalMemory.h"


int PhysicalMemory::getValidPageIndex()
{
    if (this->physicalPageCounter != this->numPhysical)
        return this->physicalPageCounter++;

    int i = 0;
    for (; i < this->numPhysical; ++i)
        if (this->valids[i])
            return i;

    return -1;
}

PhysicalMemory::~PhysicalMemory()
{
    if (this->pages != NULL)
    {
        for (int i = 0; i < this->numPhysical; ++i)
            delete [] this->pages[i];

        delete [] this->pages;
        delete [] this->valids;
    }
}

PhysicalMemory::PhysicalMemory(int frameSize, int numPhysical)
{
    this->numPhysical = numPhysical;
    this->frameSize = frameSize;
    this->physicalMemorySize = this->frameSize * this->numPhysical;
    this->valids = new bool[this->numPhysical];
    this->pages = new int*[this->numPhysical];
    for (int i = 0; i < this->numPhysical; ++i)
    {
        this->pages[i] = new int [this->frameSize];
        this->valids[i] = true;
    }
}
