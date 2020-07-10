/*
    CSE 312
    Operating Systems
    Final Project
    virtualMemory.cpp
    Omer CEVIK
    161044004
*/

#include "virtualMemory.h"

bool VirtualMemory::checkConstraints(unsigned int index, char* tName)
{
    if (index < 0 || index > this->getVirtualMemorySize())
    {
        cerr << "Error: Unacceptable index!" << endl;
        return false;
    }
    string threadName(tName);

    if (threadName.compare("bubble") != 0 && threadName.compare("index") != 0 && threadName.compare("merge") != 0 && threadName.compare("quick") != 0 )
    {
        cerr << "Error: Unacceptable thread name! " << threadName << endl;
        return false;
    }

    if (this->allocPolicy.compare("global") == 0)
        return true;

    int whichPage = index / this->getFrameSize();

    if (threadName.compare("bubble") == 0 && whichPage < this->getVirtualSize()/4)
        return true;
    else if (threadName.compare("quick") == 0 && (whichPage >= this->getVirtualSize()/4) && (whichPage < this->getVirtualSize()/2))
        return true;
    else if (threadName.compare("merge") == 0 && (whichPage >= this->getVirtualSize()/2 && whichPage < 3*this->getVirtualSize()/4))
        return true;
    else if (threadName.compare("index") == 0 && (whichPage >= 3*this->getVirtualSize()/4 && whichPage < this->getVirtualSize()))
        return true;

    cerr << "Error: Unacceptable local index in " << threadName << endl;
    return false;
}

int VirtualMemory::searchInPageTableGet(int vmPageIndex, int InPageIndex)
{
    int i = 0, currentPageInPT = 0;
    bool isHit = false;
    for (; i < this->pt.getTableCounter(); ++i)
    {
        if (this->pt.getPageTable(i).getPageIndex() == vmPageIndex)
        {
            currentPageInPT = i;
            isHit = true;
            break;
        }
    }

    if (isHit)
    {
        // Page Hit
        ++this->numberOfreads;
        ++this->pageTablePrintIntCounter;

        if (this->pageReplacement.compare("LRU") == 0 || this->pageReplacement.compare("SC") == 0
            || this->pageReplacement.compare("FIFO") == 0 || this->pageReplacement.compare("WSCLOCK") == 0)
        {
            int queueIndex = 0;
            for (; queueIndex < this->pt.getQueue().size(); ++queueIndex)
                if (this->pt.getQueue().at(queueIndex) == vmPageIndex)
                    break;

            if (this->pageReplacement.compare("LRU") == 0 || this->pageReplacement.compare("WSCLOCK") == 0)
            {
                auto it = this->pt.getQueue().begin();
                for (int k = queueIndex; k > 0; ++it, --k);
                this->pt.getQueue().erase(it);
                this->pt.insertQueue(vmPageIndex);
                this->pt.setWsClockPointer(vmPageIndex);
            }
        }
        if (this->pageReplacement.compare("SC") == 0 || this->pageReplacement.compare("NRU") == 0 || this->pageReplacement.compare("WSCLOCK") == 0)
            this->pt.getPageTable(currentPageInPT).setReferenced(true);

        this->pt.getPageTable(currentPageInPT).setModified(false);


        return this->pm->getPages()[this->pt.getPageTable(currentPageInPT).getPageIndexPM()][InPageIndex];
    }
    else
    {
        // Page Miss
        ++this->numberOfPageMisses;
        this->numberOfwrites += this->frameSize;
        this->pageTablePrintIntCounter += this->frameSize;

        vector<int> virtualPageValues;
        for (i = 0; i < this->frameSize; ++i, ++this->numberOfreads)
            virtualPageValues.push_back(this->pages[vmPageIndex][i]);

        if (this->pt.getTableCounter() < this->pt.getMAX())
        {
            // Guarantee Page Miss
            int validIndexPM = this->pm->getValidPageIndex();
            int pageTableIndex = this->pt.getTableCounter();

            this->pt.getPageTable(pageTableIndex).setModified(true);
            this->pt.getPageTable(pageTableIndex).setReferenced(false);
            this->pt.getPageTable(pageTableIndex).setPageIndexPM(validIndexPM);
            this->pt.getPageTable(pageTableIndex).setPageIndex(vmPageIndex);
            this->pt.insertQueue(vmPageIndex);
            this->pt.setWsClockPointer(vmPageIndex);
            this->pt.setTableCounter(this->pt.getTableCounter() + 1);

            for (i = 0; i < virtualPageValues.size(); ++i)
                this->pm->getPages()[validIndexPM][i] = virtualPageValues.at(i);

            this->pm->setValidAt(validIndexPM, false);

            return this->pm->getPages()[this->pt.getPageTable(pageTableIndex).getPageIndexPM()][InPageIndex];
        }
        // Page Replacement algorithm!!
        ++this->numberOfPageReplacements;
        if (vmPageIndex < this->numVirtual/4)
            ++this->bubblePageReplacementCounter;
        else if ((vmPageIndex >= this->numVirtual/4) && (vmPageIndex < this->numVirtual/2))
            ++this->mergePageReplacementCounter;
        else if ((vmPageIndex >= this->numVirtual/2) && (vmPageIndex < 3*this->numVirtual/4))
            ++this->quickPageReplacementCounter;
        else
            ++this->indexPageReplacementCounter;

        this->pr.pageReplace(this->pageReplacement, this->pt, vmPageIndex);
        int validIndexPM = this->pr.getPMIndex();
        for (i = 0; i < virtualPageValues.size(); ++i)
            this->pm->getPages()[validIndexPM][i] = virtualPageValues.at(i);

        this->numberOfDiskPageWrites += this->frameSize;
        this->numberOfDiskPageReads += this->frameSize;


        return this->pm->getPages()[validIndexPM][InPageIndex];
    }
}

void VirtualMemory::searchInPageTableSet(int vmPageIndex, int valueIndex, int value)
{
    int i = 0, currentPageInPT = 0;
    bool isHit = false;
    for (; i < this->pt.getTableCounter(); ++i)
    {
        if (this->pt.getPageTable(i).getPageIndex() == vmPageIndex)
        {
            currentPageInPT = i;
            isHit = true;
            break;
        }
    }

    if (isHit)
    {
        // Page Hit
        ++this->numberOfreads;
        ++this->numberOfwrites;
        ++this->pageTablePrintIntCounter;

        if (this->pageReplacement.compare("LRU") == 0 || this->pageReplacement.compare("SC") == 0
            || this->pageReplacement.compare("FIFO") == 0 || this->pageReplacement.compare("WSCLOCK") == 0)
        {
            int queueIndex = 0;
            for (; queueIndex < this->pt.getQueue().size(); ++queueIndex)
                if (this->pt.getQueue().at(queueIndex) == vmPageIndex)
                    break;

            if (this->pageReplacement.compare("LRU") == 0 || this->pageReplacement.compare("WSCLOCK") == 0)
            {
                auto it = this->pt.getQueue().begin();
                for (int k = queueIndex; k > 0; ++it, --k);
                this->pt.getQueue().erase(it);
                this->pt.insertQueue(vmPageIndex);
                this->pt.setWsClockPointer(vmPageIndex);
            }
        }
        if (this->pageReplacement.compare("SC") == 0 || this->pageReplacement.compare("NRU") == 0 || this->pageReplacement.compare("WSCLOCK") == 0)
            this->pt.getPageTable(currentPageInPT).setReferenced(true);
        this->pt.getPageTable(currentPageInPT).setModified(false);

        this->pages[vmPageIndex][valueIndex] = value;
        this->pm->getPages()[this->pt.getPageTable(currentPageInPT).getPageIndexPM()][valueIndex] = value;
    }
    else
    {
        // Page Miss
        ++this->numberOfPageMisses;
        this->numberOfwrites += this->frameSize;
        this->pageTablePrintIntCounter += this->frameSize;

        this->pages[vmPageIndex][valueIndex] = value;

        vector<int> virtualPageValues;
        for (i = 0; i < this->frameSize; ++i, ++this->numberOfreads)
            virtualPageValues.push_back(this->pages[vmPageIndex][i]);

        if (this->pt.getTableCounter() < this->pt.getMAX())
        {
            // Guarantee Page Miss
            int validIndexPM = this->pm->getValidPageIndex();
            int pageTableIndex = this->pt.getTableCounter();

            this->pt.getPageTable(pageTableIndex).setModified(true);
            this->pt.getPageTable(pageTableIndex).setReferenced(false);
            this->pt.getPageTable(pageTableIndex).setPageIndexPM(validIndexPM);
            this->pt.getPageTable(pageTableIndex).setPageIndex(vmPageIndex);
            this->pt.insertQueue(vmPageIndex);
            this->pt.setWsClockPointer(vmPageIndex);
            this->pt.setTableCounter(this->pt.getTableCounter() + 1);

            for (i = 0; i < virtualPageValues.size(); ++i)
                this->pm->getPages()[validIndexPM][i] = virtualPageValues.at(i);

            this->pm->setValidAt(validIndexPM, false);

            this->pm->getPages()[this->pt.getPageTable(pageTableIndex).getPageIndexPM()][valueIndex] = value;
        }
        else
        {
            // Page Replacement algorithm!!
            if (vmPageIndex < this->numVirtual/4)
                ++this->bubblePageReplacementCounter;
            else if ((vmPageIndex >= this->numVirtual/4) && (vmPageIndex < this->numVirtual/2))
                ++this->mergePageReplacementCounter;
            else if ((vmPageIndex >= this->numVirtual/2) && (vmPageIndex < 3*this->numVirtual/4))
                ++this->quickPageReplacementCounter;
            else
                ++this->indexPageReplacementCounter;

            ++this->numberOfPageReplacements;
            this->pr.pageReplace(this->pageReplacement, this->pt, vmPageIndex);
            int validIndexPM = this->pr.getPMIndex();
            for (i = 0; i < virtualPageValues.size(); ++i)
                this->pm->getPages()[validIndexPM][i] = virtualPageValues.at(i);

            this->pm->getPages()[validIndexPM][valueIndex] = value;
            this->numberOfDiskPageWrites += this->frameSize;
            this->numberOfDiskPageReads += this->frameSize;
        }
    }
}


VirtualMemory::VirtualMemory(int numVirtual, int numPhysical, int frameSize, int pageTablePrintInt, string allocPolicy, string pageReplacement, string diskFileName)
{
    this->numVirtual = numVirtual;
    this->frameSize = frameSize;
    this->pageTablePrintInt = pageTablePrintInt;
    this->pageReplacement = pageReplacement;
    this->allocPolicy = allocPolicy;
    this->diskFileName = diskFileName;
    this->virtualMemorySize = this->frameSize * this->numVirtual;
    this->pm = new PhysicalMemory(this->frameSize, numPhysical);
    int pageTableSize = this->virtualMemorySize / this->pm->getPhysicalMemorySize();
    if (pageTableSize >= numPhysical)
        pageTableSize /= 2;
    this->pt.initTable(pageTableSize);
    this->pages = new int*[this->numVirtual];
    this->pr.openDiskFile(this->diskFileName);
    this->pr.setPhysicalMemory(pm);
    for (int i = 0; i < this->numVirtual; ++i)
        this->pages[i] = new int[this->frameSize];
}

void VirtualMemory::printPageTable()
{
    if (this->pageTablePrintIntCounter >= this->pageTablePrintInt)
    {
        cout << "-------------- Page Table --------------" << endl;
        cout << "\tCurrent Page Table Size : " << this->pt.getTableCounter() << endl;
        cout << "\tMaximum Page Table Size : " << this->pt.getMAX() << endl;
        int i = 0;
        cout << "\tPage Table Bits and Indexes : " << endl;
        for (; i < this->pt.getTableCounter(); ++i)
        {
            cout << "\t\t" << "Page Table Entry [" << i << "] -> " << "Modified Bit   : " << this->pt.getPageTable(i).getModified() << endl;
            cout << "\t\t" << "Page Table Entry [" << i << "] -> " << "Referenced Bit : " << this->pt.getPageTable(i).getReferenced() << endl;
            cout << "\t\t" << "Page Table Entry [" << i << "] -> " << "Virtual  Memory Page Index : " << this->pt.getPageTable(i).getPageIndex() << endl;
            cout << "\t\t" << "Page Table Entry [" << i << "] -> " << "Physical Memory Page Index : " << this->pt.getPageTable(i).getPageIndexPM() << endl;
        }
        if (this->pageReplacement.compare("SC") || this->pageReplacement.compare("FIFO")
         || this->pageReplacement.compare("LRU") || this->pageReplacement.compare("WSCLOCK"))
        {
            cout << "\tThe Current Queue For " << this->pageReplacement << " :" << endl;
            cout << "\t\t";
            int j = 0;
            for (; j < this->pt.getQueue().size(); ++j)
                cout << this->pt.getQueue().at(j) << ", ";
            cout << endl;
        }
        cout << "----------------------------------------" << endl << flush;

        this->pageTablePrintIntCounter = 0;
    }
}

void VirtualMemory::printStatistics()
{
    cout << "-------------- Program Statistics --------------" << endl;
    cout << "Number of Reads\t\t: " << numberOfreads << endl;
    cout << "Number of Writes\t\t: " << numberOfwrites << endl;
    cout << "Number of Page Misses\t\t: " << numberOfPageMisses << endl;
    cout << "Number of Page Replacements\t\t: " << numberOfPageReplacements << endl;
    cout << "Number of Disk Page Writes\t\t: " << numberOfDiskPageWrites << endl;
    cout << "Number of Disk Page Reads\t\t: " << numberOfDiskPageReads << endl;
    cout << "------------------------------------------------" << endl << flush;
}

void VirtualMemory::isAllSorted()
{
    int i = 0, j = 0;
    bool notSortedBubble = false, notSortedQuick = false,
         notSortedMerge = false, notSortedIndex = false;

    for (; i < (this->numVirtual/4) && !notSortedBubble; ++i)
        for (j = 0; j < this->frameSize-1; ++j)
            if (this->pages[i][j] > this->pages[i][j+1])
            {
                notSortedBubble = true;
                break;
            }

    for (i = this->numVirtual/4; i < (this->numVirtual/2) && !notSortedQuick; ++i)
        for (j = 0; j < this->frameSize-1; ++j)
            if (this->pages[i][j] > this->pages[i][j+1])
            {
                notSortedQuick = true;
                break;
            }

    for (i = this->numVirtual/2; i < (3*this->numVirtual/4) && !notSortedMerge; ++i)
        for (j = 0; j < this->frameSize-1; ++j)
            if (this->pages[i][j] > this->pages[i][j+1])
            {
                notSortedMerge = true;
                break;
            }

    for (i = 3*this->numVirtual/4; i < this->numVirtual && !notSortedIndex; ++i)
        for (j = 0; j < this->frameSize-1; ++j)
            if (this->pages[i][j] > this->pages[i][j+1])
            {
                notSortedIndex = true;
                break;
            }


    if (notSortedBubble)
        cout << "Bubble Sort is not sorted!" << endl;
    else
        cout << "Bubble Sort is sorted successfully!" << endl;

    if (notSortedQuick)
        cout << "Quick Sort is not sorted!" << endl;
    else
        cout << "Quick Sort is sorted successfully!" << endl;

    if (notSortedMerge)
        cout << "Merge Sort is not sorted!" << endl;
    else
        cout << "Merge Sort is sorted successfully!" << endl;

    if (notSortedIndex)
        cout << "Index Sort is not sorted!" << endl;
    else
        cout << "Index Sort is sorted successfully!" << endl;
}

void VirtualMemory::fillRandoms()
{
    int i = 0, j = 0;
    srand(1000);
    for (; i < this->numVirtual; ++i)
        for (j = 0; j < this->frameSize; ++j)
            this->pages[i][j] = rand();
}

VirtualMemory::~VirtualMemory()
{
    if (this->pm != NULL)
    {
        for (int i = 0; i < this->numVirtual; ++i)
            delete [] this->pages[i];
        delete [] this->pages;
        delete this->pm;
    }
}