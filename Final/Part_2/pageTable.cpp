/*
    CSE 312
    Operating Systems
    Final Project
    pageTable.cpp
    Omer CEVIK
    161044004
*/

#include "pageTable.h"

PageTableEntry::PageTableEntry(PageTableEntry& pte)
{
    this->pageIndex = pte.getPageIndex();
    this->pmPageIndex = pte.getPageIndexPM();
    this->modified = pte.getModified();
    this->referenced = pte.getReferenced();
}

void PageTable::initTable(int max)
{
    this->setMAX(max);
    this->table = new PageTableEntry[this->getMAX()];
}

PageTable::~PageTable()
{
    delete [] this->table;
}