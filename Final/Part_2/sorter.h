/*
    CSE 312
    Operating Systems
    Final Project
    sorter.h
    Omer CEVIK
    161044004
*/

#ifndef SORTER_H
#define SORTER_H

#include "memory.h"
#include "virtualMemory.h"
#include <ctime>
#include <mutex>

void set(unsigned int index, int value, char * tName);
int get(unsigned int index, char * tName);

class Sorter
{
public:
    Sorter(){}
    Sorter(VirtualMemory* vm);
    void bubbleSort();
    void bubbleSortAlgorithm(int begin, int end);

    void quickSort();
    void quickSortAlgorithm(int low, int high);

    void mergeSort();
    void mergeSortAlgorithm(int l, int r);

    void indexSort();
    void indexSortAlgorithm(int begin, int end);

private:
    VirtualMemory* vm;
    int quarterSize;
    string bubbleString = "bubble";
    string quickString = "quick";
    string mergeString = "merge";
    string indexString = "index";
};

#endif