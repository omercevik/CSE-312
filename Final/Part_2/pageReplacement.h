/*
    CSE 312
    Operating Systems
    Final Project
    pageReplacement.h
    Omer CEVIK
    161044004
*/

#ifndef PAGE_REPLACEMENT_H
#define PAGE_REPLACEMENT_H

#include <iostream>
#include <cstdlib>
#include <fstream>

#include "pageTable.h"
#include "physicalMemory.h"

using namespace std;

class PageReplacement
{
public:
    PageReplacement(){}
    void pageReplace(string pageReplacement, PageTable& pt, int pageIndex);
    void NRUReplacement(PageTable& pt, int pageIndex);
    void FIFOReplacement(PageTable& pt, int pageIndex);
    void SCReplacement(PageTable& pt, int pageIndex);
    void LRUReplacement(PageTable& pt, int pageIndex);
    void WSClockReplacement(PageTable& pt, int pageIndex);

    void openDiskFile(string diskFileName);
    void closeDiskFile();
    void writePageToDisk(int removedPageIndex);
    void setPhysicalMemory(PhysicalMemory* pm) { this->pm = pm; }

    int getPMIndex() { return this->pageIndexPM; }
private:
    PhysicalMemory* pm = NULL;
    ofstream diskFile;
    int pageIndexPM = 0;
};

#endif