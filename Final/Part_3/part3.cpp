/*
    CSE 312
    Operating Systems
    Final Project
    part3.cpp
    Omer CEVIK
    161044004
*/

#include "sorter.h"
#include "physicalMemory.h"
#include "virtualMemory.h"
#include <thread>

using namespace std;

VirtualMemory* vm = NULL;

void getArguments(char const *argv[], string* diskFileName, string* pageReplacement, string* allocPolicy);

int main(int argc, char const *argv[])
{
    if (argc != 4)
    {
        cerr << "Unacceptable arguments!" << endl;
        cerr << "Usage : ./sortArrays pageReplacement allocPolicy diskFileName" << endl;
        exit(EXIT_FAILURE);
    }

    int frameSize = 2, numPhysical = 16*1024, numVirtual = 128*1024, pageTablePrintInt = 0;
    string diskFileName = "", pageReplacement = "", allocPolicy = "";

    getArguments(argv, &diskFileName, &pageReplacement, &allocPolicy);

    int optimalFrameSize = frameSize;
    vector<int> frames, bubbleVector, mergeVector, indexVector, quickVector;
    int i = 0, j = 0;
    int minBubble = 999999, minMerge = 999999, minIndex = 999999, minQuick = 999999;
    int optFrameBubble = 999999, optFrameMerge = 999999, optFrameIndex = 999999, optFrameQuick = 999999;

    for (; frameSize <= 4096; frameSize *= 2)
    {
        int numP = numPhysical / frameSize;
        int numV = numVirtual / frameSize;

        vm = new VirtualMemory(numV, numP, frameSize, pageTablePrintInt, allocPolicy, pageReplacement, diskFileName);
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

        bubbleVector.push_back(vm->getBubblePRC());
        quickVector.push_back(vm->getQuickPRC());
        mergeVector.push_back(vm->getMergePRC());
        indexVector.push_back(vm->getIndexPRC());

        if (vm->getBubblePRC() < minBubble)
        {
            optFrameBubble = frameSize;
            minBubble = vm->getBubblePRC();
        }
        if (vm->getMergePRC() < minMerge)
        {
            optFrameMerge = frameSize;
            minMerge = vm->getMergePRC();
        }
        if (vm->getQuickPRC() < minQuick)
        {
            optFrameQuick = frameSize;
            minQuick = vm->getQuickPRC();
        }
        if (vm->getIndexPRC() < minIndex)
        {
            optFrameIndex = frameSize;
            minIndex = vm->getIndexPRC();
        }

        frames.push_back(frameSize);

        delete vm;
    }

    cout << "Bubble Sort" << endl;
    for (i = 0; i < bubbleVector.size(); ++i)
        cout << "\tFrame : " << frames[i] << " Page Replacement Size : " << bubbleVector[i] << endl;
    cout << "Bubble Sort the optimal frame is " << optFrameBubble << endl << endl;

    cout << "Quick Sort" << endl;
    for (i = 0; i < quickVector.size(); ++i)
        cout << "\tFrame : " << frames[i] << " Page Replacement Size : " << quickVector[i] << endl;
    cout << "Quick Sort the optimal frame is " << optFrameBubble << endl << endl;

    cout << "Merge Sort" << endl;
    for (i = 0; i < mergeVector.size(); ++i)
        cout << "\tFrame : " << frames[i] << " Page Replacement Size : " << mergeVector[i] << endl;
    cout << "Merge Sort the optimal frame is " << optFrameBubble << endl << endl;

    cout << "Index Sort" << endl;
    for (i = 0; i < indexVector.size(); ++i)
        cout << "\tFrame : " << frames[i] << " Page Replacement Size : " << indexVector[i] << endl;
    cout << "Index Sort the optimal frame is " << optFrameBubble << endl << endl;


    // ************************************ BONUS 1 ************************************

    cout << "************************************ BONUS PART ************************************" << endl << endl;


    frameSize = 128;

    cout << "Frame Size is : " << frameSize << endl;

    int numP = numPhysical / frameSize;
    int numV = numVirtual / frameSize;
    int bestPRbubble = 999999, bestPRmerge = 999999,  bestPRquick = 999999, bestPRindex = 999999;
    int bestPRbubbleIndex = 0, bestPRmergeIndex = 0,  bestPRquickIndex = 0, bestPRindexIndex = 0;
    string prStrings[5] = { "LRU", "FIFO", "NRU", "SC", "WSCLOCK" };


    for (j = 0; j < 5; ++j)
    {
        bubbleVector.clear();
        mergeVector.clear();
        indexVector.clear();
        quickVector.clear();

        for (i = 0; i < 10; ++i)
        {
            vm = new VirtualMemory(numV, numP, frameSize, pageTablePrintInt, allocPolicy, prStrings[j], diskFileName);

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

            bubbleVector.push_back(vm->getBubblePRC());
            quickVector.push_back(vm->getQuickPRC());
            mergeVector.push_back(vm->getMergePRC());
            indexVector.push_back(vm->getIndexPRC());

            vm->closeDiskFile();

            delete vm;
        }


        int avgBubble = 0, avgMerge = 0, avgIndex = 0, avgQuick = 0;

        for (i = 0; i < bubbleVector.size(); ++i)
        {
            avgBubble += bubbleVector[i];
            avgQuick += quickVector[i];
            avgMerge += mergeVector[i];
            avgIndex += indexVector[i];
        }
        avgBubble /= bubbleVector.size();
        avgQuick /= quickVector.size();
        avgMerge /= mergeVector.size();
        avgIndex /= indexVector.size();

        cout << prStrings[j] << " Results:" << endl;
        cout << "\tBubble Sort average of page replacements is " << avgBubble << endl;
        cout << "\tQuick Sort average of page replacements is " << avgQuick << endl;
        cout << "\tMerge Sort average of page replacements is " << avgMerge << endl;
        cout << "\tIndex Sort average of page replacements is " << avgIndex << endl;

        if (avgBubble < bestPRbubble)
        {
            bestPRbubbleIndex = j;
            bestPRbubble = avgBubble;
        }
        if (avgQuick < bestPRquick)
        {
            bestPRquickIndex = j;
            bestPRquick = avgQuick;
        }
        if (avgMerge < bestPRmerge)
        {
            bestPRmergeIndex = j;
            bestPRmerge = avgMerge;
        }
        if (avgIndex < bestPRindex)
        {
            bestPRindexIndex = j;
            bestPRindex = avgIndex;
        }
    }

    cout << endl;
    cout << "Bubble Sort best page replacement is " << prStrings[bestPRbubbleIndex] << " and size is " << bestPRbubble <<  endl;
    cout << "Quick Sort best page replacement is " << prStrings[bestPRquickIndex] << " and size is " << bestPRquick <<  endl;
    cout << "Merge Sort best page replacement is " << prStrings[bestPRmergeIndex] << " and size is " << bestPRmerge <<  endl;
    cout << "Index Sort best page replacement is " << prStrings[bestPRquickIndex] << " and size is " << bestPRindex <<  endl;

    return 0;
}

void getArguments(char const *argv[], string* diskFileName, string* pageReplacement, string* allocPolicy)
{
    string temp1(argv[1]);
    *pageReplacement = temp1;

    string temp2(argv[2]);
    *allocPolicy = temp2;

    string temp3(argv[3]);
    *diskFileName = temp3;

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