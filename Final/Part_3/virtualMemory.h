/*
    CSE 312
    Operating Systems
    Final Project
    virtualMemory.h
    Omer CEVIK
    161044004
*/

#ifndef VIRTUAL_MEMORY_H
#define VIRTUAL_MEMORY_H

#include "physicalMemory.h"
#include "pageTable.h"
#include "pageReplacement.h"

class VirtualMemory
{
public:
    VirtualMemory(){}
    ~VirtualMemory();
    VirtualMemory(int numVirtual, int numPhysical, int frameSize, int pageTablePrintInt, string allocPolicy, string pageReplacement, string diskFileName);

    void fillRandoms();
    const int getVirtualSize()const { return this->numVirtual; }
    const int getVirtualMemorySize()const { return this->virtualMemorySize; }

    PhysicalMemory* getPm() { return this->pm; }
    PageTable getPt() { return this->pt; }
    int getPageTablePrintInt() { return this->pageTablePrintInt; }
    string getAllocPolicy() { return this->allocPolicy; }
    string getPageReplacement() { return this->pageReplacement; }
    int getFrameSize() { return this->frameSize; }
    string getDiskFileName() { return this->diskFileName; }

    void printPageTable();
    void printStatistics();
    void isAllSorted();


    int getBubblePRC() { return this->bubblePageReplacementCounter; }
    int getMergePRC() { return this->mergePageReplacementCounter; }
    int getQuickPRC() { return this->quickPageReplacementCounter; }
    int getIndexPRC() { return this->indexPageReplacementCounter; }

    bool checkConstraints(unsigned int index, char* tName);
    int searchInPageTableGet(int vmPageIndex, int InPageIndex);
    void searchInPageTableSet(int vmPageIndex, int valueIndex, int value);

    void closeDiskFile() { pr.closeDiskFile(); }

    int getNumberOfPageReplacements() { return this->numberOfPageReplacements; }

    int** getPages() { return this->pages; }

private:
    PhysicalMemory* pm = NULL;
    PageTable pt;
    PageReplacement pr;
    int** pages = NULL;

    int frameSize = -1;
    int pageTablePrintInt;
    int pageTablePrintIntCounter = 0;

    int numberOfreads = 0;
    int numberOfwrites = 0;
    int numberOfPageMisses = 0;
    int numberOfPageReplacements = 0;
    int numberOfDiskPageWrites = 0;
    int numberOfDiskPageReads = 0;
    string allocPolicy;
    string pageReplacement;
    string diskFileName;

    int numVirtual;
    int virtualMemorySize;

    int bubblePageReplacementCounter = 0;
    int mergePageReplacementCounter = 0;
    int quickPageReplacementCounter = 0;
    int indexPageReplacementCounter = 0;
};

#endif