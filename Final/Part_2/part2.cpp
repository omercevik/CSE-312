/*
    CSE 312
    Operating Systems
    Final Project
    part2.cpp
    Omer CEVIK
    161044004
*/

#include "sorter.h"
#include "physicalMemory.h"
#include "virtualMemory.h"
#include <thread>

using namespace std;

VirtualMemory* vm = NULL;

void getArguments(char const *argv[], int* frameSize, int* numPhysical, int* numVirtual, int* pageTablePrintInt,
    string* diskFileName, string* pageReplacement, string* allocPolicy);

int main(int argc, char const *argv[])
{
    if (argc != 8)
    {
        cerr << "Unacceptable arguments!" << endl;
        cerr << "Usage : ./sortArrays frameSize numPhysical numVirtual pageReplacement allocPolicy pageTablePrintInt diskFileName.dat" << endl;
        exit(EXIT_FAILURE);
    }

    int frameSize = 0, numPhysical = 0, numVirtual = 0, pageTablePrintInt = 0;
    string diskFileName = "", pageReplacement = "", allocPolicy = "";

    getArguments(argv, &frameSize, &numPhysical, &numVirtual, &pageTablePrintInt, &diskFileName, &pageReplacement, &allocPolicy);

    vm = new VirtualMemory(numVirtual, numPhysical, frameSize, pageTablePrintInt, allocPolicy, pageReplacement, diskFileName);

    vm->fillRandoms();
    Sorter sorter(vm);

    thread bubbleThread ([&]{ sorter.bubbleSort(); });
    thread quickThread ([&]{ sorter.quickSort(); });
    thread mergeThread ([&]{ sorter.mergeSort(); });
    thread indexThread ([&]{ sorter.indexSort(); });


    bubbleThread.join();
    quickThread.join();
    mergeThread.join();
    indexThread.join();

    vm->closeDiskFile();

    vm->isAllSorted();

    vm->printStatistics();

    delete vm;

    return 0;
}

void getArguments(char const *argv[], int* frameSize, int* numPhysical, int* numVirtual, int* pageTablePrintInt, string* diskFileName, string* pageReplacement, string* allocPolicy)
{
    sscanf(argv[1],"%d",frameSize);
    sscanf(argv[2],"%d",numPhysical);
    sscanf(argv[3],"%d",numVirtual);

    string temp1(argv[4]);
    *pageReplacement = temp1;

    string temp2(argv[5]);
    *allocPolicy = temp2;

    sscanf(argv[6],"%d",pageTablePrintInt);

    string temp3(argv[7]);
    *diskFileName = temp3;

    if (*frameSize < 0 || *numPhysical < 0 || *numVirtual < 0 || *pageTablePrintInt < 0)
    {
        cerr << "Error : Not positive numbers detected!" << endl;
        exit(EXIT_FAILURE);
    }
    if (allocPolicy->compare("local") != 0 && allocPolicy->compare("global") != 0)
    {
        cerr << "Error : Not local or global allocation policy detected!" << endl;
        exit(EXIT_FAILURE);
    }
    if (pageReplacement->compare("FIFO") != 0 && pageReplacement->compare("LRU") != 0 &&
        pageReplacement->compare("NRU") != 0 && pageReplacement->compare("SC") != 0 && pageReplacement->compare("WSCLOCK") != 0)
    {
        cerr << "Error : Not FIFO, LRU, NRU, SC or WSCLOCK page replacement detected!" << endl;
        exit(EXIT_FAILURE);
    }
}