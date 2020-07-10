/*
    CSE 312
    Operating Systems
    Final Project
    pageTable.h
    Omer CEVIK
    161044004
*/

#ifndef PAGE_TABLE_H
#define PAGE_TABLE_H

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class PageTableEntry
{
public:
    PageTableEntry(){}
    ~PageTableEntry(){}
    PageTableEntry(PageTableEntry& pte);

    void setPageIndex(int index) { this->pageIndex = index; }
    int getPageIndex() { return this->pageIndex; }

    void setPageIndexPM(int index) { this->pmPageIndex = index; }
    int getPageIndexPM() { return this->pmPageIndex; }

    const bool getModified() const { return this->modified; }
    const bool getReferenced() const { return this->referenced; }

    void setModified(bool modified) { this->modified = modified; }
    void setReferenced(bool referenced) { this->referenced = referenced; }

private:
    int pageIndex = -1;
    int pmPageIndex = -1;

    bool modified = false;  // NRU
    bool referenced = false; // Second-Chance & NRU & LRU & WSClock
};

class PageTable
{
public:
    PageTable(){}
    ~PageTable();

    int getMAX(){ return this->MAX_TABLE_SIZE; }
    void setMAX(int max){ this->MAX_TABLE_SIZE = max; }

    void initTable(int max);

    void insertQueue(int pageIndex) { queue.push_back(pageIndex); }
    void removeFirstFromQueue() { queue.erase(queue.begin()); }
    vector<int>& getQueue() { return this->queue; }

    int getTableCounter() { return this->tableCounter; }
    void setTableCounter(int counter) { this->tableCounter = counter; }

    int getWsClockPointer() { return this->wsClockPointer; }
    void setWsClockPointer(int wsClockPointer) { this->wsClockPointer = wsClockPointer; }

    PageTableEntry& getPageTable(int index) { return this->table[index]; }
    void setPageTable(int index, PageTableEntry pte) { this->table[index] = pte; }

private:
    PageTableEntry* table = NULL;
    vector<int> queue; // FIFO, Second-Chance, LRU, WSClock
    int wsClockPointer = 0; // WSClock ( Last Used is pointed! )
    int tableCounter = 0;
    int MAX_TABLE_SIZE = 0;
};

#endif