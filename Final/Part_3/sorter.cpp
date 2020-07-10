/*
    CSE 312
    Operating Systems
    Final Project
    sorter.cpp
    Omer CEVIK
    161044004
*/

#include "sorter.h"

extern VirtualMemory* vm;
mutex pageTableMutex;

void set(unsigned int index, int value, char * tName)
{
    if (!vm->checkConstraints(index, tName))
        return;

    int whichPage = index / vm->getFrameSize();
    int whichPageIndex = index % vm->getFrameSize();

    vm->searchInPageTableSet(whichPage, whichPageIndex, value);
}

int get(unsigned int index, char * tName)
{
    if (!vm->checkConstraints(index, tName))
        return -1;


    int whichPage = index / vm->getFrameSize();
    int whichPageIndex = index % vm->getFrameSize();

    int returnValue = vm->searchInPageTableGet(whichPage, whichPageIndex);

    return returnValue;
}

Sorter::Sorter(VirtualMemory* vm)
{
    this->vm = vm;
    this->quarterSize = vm->getVirtualSize() / 4;
}

void Sorter::bubbleSortAlgorithm(int begin, int end)
{
    int i = begin, j = begin, x = 0, y = 0;

    for (i = 0; i < (end - begin); ++i)
    {
        for (j = begin; j < (end - i - 1); ++j)
        {
            x = get(j, (char*)this->bubbleString.c_str()); // list[j]
            y = get(j+1, (char*)this->bubbleString.c_str()); // list[j+1]
            if (x > y)
            {
                set(j, y, (char*)this->bubbleString.c_str());
                set(j+1, x, (char*)this->bubbleString.c_str());
            }
        }
    }
}

void Sorter::bubbleSort()
{
    int i = 0, begin = 0, end = this->quarterSize;
    int frameMult = vm->getFrameSize();
    int beginCounter = 0, endCounter = frameMult;
    for (i = begin; i < end; ++i)
    {
        pageTableMutex.lock();

        bubbleSortAlgorithm(beginCounter, endCounter);

        pageTableMutex.unlock();

        beginCounter += frameMult;
        endCounter += frameMult;
    }
}

void Sorter::quickSortAlgorithm(int low, int high)
{
    int i = 0, j = 0, pivot = 0, temp = 0, listPivot = 0, listI = 0, listJ = 0;

    if(low < high)
    {
        pivot = low;
        i = low;
        j = high;

        while(i < j)
        {
            listI = get(i, (char*)this->quickString.c_str());
            listPivot = get(pivot, (char*)this->quickString.c_str());
            while(listI <= listPivot && i < high)
            {
                ++i;
                listI = get(i, (char*)this->quickString.c_str());
            }

            listJ = get(j, (char*)this->quickString.c_str());
            while(listJ > listPivot)
            {
                --j;
                listJ = get(j, (char*)this->quickString.c_str());
            }

            if(i < j)
            {
                temp = listI;
                listI = listJ;
                set(i, listI, (char*)this->quickString.c_str());
                listJ = temp;
                set(j, listJ, (char*)this->quickString.c_str());
            }
        }
        temp = listPivot;
        listPivot = listJ;
        set(pivot, listPivot, (char*)this->quickString.c_str());
        listJ = temp;
        set(j, listJ, (char*)this->quickString.c_str());

        quickSortAlgorithm(low, j-1);
        quickSortAlgorithm(j+1, high);
    }
}

void Sorter::quickSort()
{
    int i = 0, begin = this->quarterSize, end = this->quarterSize * 2;
    int frameMult = vm->getFrameSize();
    int beginCounter = begin * frameMult, endCounter = begin * frameMult + frameMult;
    for (i = begin; i < end; ++i)
    {
        pageTableMutex.lock();

        quickSortAlgorithm(beginCounter, endCounter-1);

        pageTableMutex.unlock();

        beginCounter += frameMult;
        endCounter += frameMult;
    }
}

void Sorter::mergeSortAlgorithm(int l, int r)
{
    int m = (r + l) / 2;
    if (l < r)
    {
        mergeSortAlgorithm(l, m);
        mergeSortAlgorithm(m + 1, r);
    }

    int i = 0, j = 0, k = 0;
    int n1 = m - l + 1;
    int n2 =  r - m;

    int L[n1], R[n2];

    for (i = 0; i < n1; ++i)
        L[i] = get(l+i, (char*)this->mergeString.c_str()); // list[l + i]
    for (j = 0; j < n2; ++j)
        R[j] = get(m+1+j, (char*)this->mergeString.c_str());// list[m + 1 + j]

    i = 0;
    j = 0;
    k = l;
    while (i < n1 && j < n2)
    {
        if (L[i] <= R[j])
        {
            set(k, L[i], (char*)this->mergeString.c_str());// list[k] = L[i]
            ++i;
        }
        else
        {
            set(k, R[j], (char*)this->mergeString.c_str());// list[k] = R[j]
            ++j;
        }
        ++k;
    }

    while (i < n1)
    {
        set(k, L[i], (char*)this->mergeString.c_str());// list[k] = L[i]
        ++i;
        ++k;
    }

    while (j < n2)
    {
        set(k, R[j], (char*)this->mergeString.c_str());// list[k] = R[j]
        ++j;
        ++k;
    }
}

void Sorter::mergeSort()
{
    int i = 0, begin = this->quarterSize*2, end = 3 * this->quarterSize;
    int frameMult = vm->getFrameSize();
    int beginCounter = begin * frameMult, endCounter = begin * frameMult + frameMult;
    for (i = begin; i < end; ++i)
    {
        pageTableMutex.lock();

        mergeSortAlgorithm(beginCounter, endCounter-1);

        pageTableMutex.unlock();

        beginCounter += frameMult;
        endCounter += frameMult;
    }
}

void Sorter::indexSortAlgorithm(int begin, int end)
{
    int i = 0, j = 0, temp = 0, temp2 = 0, frameSize = vm->getFrameSize();
    int beginTemp = begin;
    int indexArray[frameSize];

    for(i = 0; i < frameSize; ++i, ++beginTemp)
        indexArray[i] = beginTemp;

    for(i = 0; i < frameSize-1; ++i)
    {
        for(j = i + 1; j < frameSize; ++j)
        {
            int x = get(indexArray[i], (char*)this->indexString.c_str());
            int y = get(indexArray[j], (char*)this->indexString.c_str());
            if(x > y) // list[indexArray[i]] > list[indexArray[j]]
            {
                temp = indexArray[i];
                indexArray[i] = indexArray[j];
                indexArray[j] = temp;
            }
        }
    }

    int tempArray[frameSize];

    for (i = 0; i < frameSize; ++i)
        tempArray[i] = get(indexArray[i], (char*)this->indexString.c_str());
    for (i = 0; i < frameSize; ++i, ++begin)
        set(begin, tempArray[i], (char*)this->indexString.c_str());
}

void Sorter::indexSort()
{
    int i = 0, begin = this->quarterSize*3, end = 4 * this->quarterSize;
    int frameMult = vm->getFrameSize();
    int beginCounter = begin * frameMult, endCounter = begin * frameMult + frameMult;
    for (i = begin; i < end; ++i)
    {
        pageTableMutex.lock();

        indexSortAlgorithm(beginCounter, endCounter-1);

        pageTableMutex.unlock();

        beginCounter += frameMult;
        endCounter += frameMult;
    }
}
