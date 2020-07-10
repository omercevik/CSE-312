/*
    CSE 312
    Operating Systems
    Final Project
    pageReplacement.cpp
    Omer CEVIK
    161044004
*/

#include "pageReplacement.h"

void PageReplacement::pageReplace(string pageReplacement, PageTable& pt, int pageIndex)
{
    if (pageReplacement.compare("NRU") == 0)
        this->NRUReplacement(pt, pageIndex);
    else if (pageReplacement.compare("FIFO") == 0)
        this->FIFOReplacement(pt, pageIndex);
    else if (pageReplacement.compare("SC") == 0)
        this->SCReplacement(pt, pageIndex);
    else if (pageReplacement.compare("LRU") == 0)
        this->LRUReplacement(pt, pageIndex);
    else if (pageReplacement.compare("WSCLOCK") == 0)
        this->WSClockReplacement(pt, pageIndex);
    else
    {
        cerr << "Error: Unacceptable replacement algorithm!" << endl;
        exit(EXIT_FAILURE);
    }
}

void PageReplacement::NRUReplacement(PageTable& pt, int pageIndex)
{
    int i = 0;

    for (; i < pt.getMAX(); ++i)
    {
        if (pt.getPageTable(i).getModified() == 0 && pt.getPageTable(i).getReferenced() == 0)
            break;
    }
    if (i == pt.getMAX())
        i = 0;

    writePageToDisk(i);

    pt.getPageTable(i).setPageIndex(pageIndex);

    this->pm->setValidAt(pt.getPageTable(i).getPageIndexPM(), true);
    int validPMIndex = this->pm->getValidPageIndex();
    this->pageIndexPM = validPMIndex;

    pt.getPageTable(i).setPageIndexPM(validPMIndex);
}

void PageReplacement::FIFOReplacement(PageTable& pt, int pageIndex)
{
    int replaceIndex = pt.getQueue().at(0), pageTableIndex = 0;
    for (pageTableIndex = 0; pageTableIndex < pt.getMAX(); ++pageTableIndex)
        if (replaceIndex == pt.getPageTable(pageTableIndex).getPageIndex())
            break;

    PageTableEntry ptReplace = pt.getPageTable(pageTableIndex);
    this->pm->setValidAt(ptReplace.getPageIndexPM(), true);

    int validPMIndex = this->pm->getValidPageIndex();
    this->pageIndexPM = validPMIndex;

    ptReplace.setPageIndex(pageIndex);
    ptReplace.setPageIndexPM(validPMIndex);

    auto it = pt.getQueue().begin();
    pt.getQueue().erase(it);
    pt.insertQueue(pageIndex);

    writePageToDisk(ptReplace.getPageIndexPM());

    pt.setPageTable(pageTableIndex, ptReplace);
}

void PageReplacement::SCReplacement(PageTable& pt, int pageIndex)
{
    int replaceIndex = pt.getQueue().at(0), pageTableIndex = 0;
    for (pageTableIndex = 0; pageTableIndex < pt.getMAX(); ++pageTableIndex)
        if (replaceIndex == pt.getPageTable(pageTableIndex).getPageIndex())
            break;

    PageTableEntry ptReplace = pt.getPageTable(pageTableIndex);
    this->pm->setValidAt(ptReplace.getPageIndexPM(), true);

    int validPMIndex = this->pm->getValidPageIndex();
    this->pageIndexPM = validPMIndex;

    auto it = pt.getQueue().begin();

    if (ptReplace.getReferenced())
    {
        replaceIndex = pt.getQueue().at(1);
        ptReplace.setReferenced(false);
        ++it;
    }
    for (pageTableIndex = 0; pageTableIndex < pt.getMAX(); ++pageTableIndex)
        if (replaceIndex == pt.getPageTable(pageTableIndex).getPageIndex())
            break;

    pt.getQueue().erase(it);
    pt.insertQueue(pageIndex);
    ptReplace.setPageIndex(pageIndex);
    ptReplace.setPageIndexPM(validPMIndex);

    writePageToDisk(ptReplace.getPageIndexPM());

    pt.setPageTable(pageTableIndex, ptReplace);
}

void PageReplacement::LRUReplacement(PageTable& pt, int pageIndex)
{
    int replaceIndex = pt.getQueue().at(0), pageTableIndex = 0;
    for (pageTableIndex = 0; pageTableIndex < pt.getMAX(); ++pageTableIndex)
        if (replaceIndex == pt.getPageTable(pageTableIndex).getPageIndex())
            break;
    PageTableEntry ptReplace = pt.getPageTable(pageTableIndex);
    this->pm->setValidAt(ptReplace.getPageIndexPM(), true);

    int validPMIndex = this->pm->getValidPageIndex();
    this->pageIndexPM = validPMIndex;

    writePageToDisk(ptReplace.getPageIndexPM());

    ptReplace.setPageIndex(pageIndex);
    ptReplace.setPageIndexPM(validPMIndex);

    auto it = pt.getQueue().begin();
    pt.getQueue().erase(it);
    pt.insertQueue(pageIndex);

    pt.setPageTable(pageTableIndex, ptReplace);
}

void PageReplacement::WSClockReplacement(PageTable& pt, int pageIndex)
{
    int i = pt.getQueue().size()-1, pageTableIndex = 0, replaceIndex = 0;

    for (; i >= 0; --i)
    {
        replaceIndex = pt.getQueue().at(i);
        for (pageTableIndex = 0; pageTableIndex < pt.getMAX(); ++pageTableIndex)
            if (replaceIndex == pt.getPageTable(pageTableIndex).getPageIndex())
                break;
        if (pt.getPageTable(pageTableIndex).getReferenced())
        {
            pt.getPageTable(pageTableIndex).setReferenced(false);
        }
        else
        {
            this->pm->setValidAt(pt.getPageTable(pageTableIndex).getPageIndexPM(), true);
            int validPMIndex = this->pm->getValidPageIndex();
            this->pageIndexPM = validPMIndex;

            pt.getPageTable(pageTableIndex).setPageIndex(pageIndex);
            pt.getPageTable(pageTableIndex).setPageIndexPM(validPMIndex);
            auto it = pt.getQueue().begin() + i;
            pt.getQueue().erase(it);
            pt.insertQueue(pageIndex);

            replaceIndex = pt.getQueue().at(pt.getQueue().size()-1);
            for (pageTableIndex = 0; pageTableIndex < pt.getMAX(); ++pageTableIndex)
                if (replaceIndex == pt.getPageTable(pageTableIndex).getPageIndex())
                    break;
            pt.getPageTable(pageTableIndex).setReferenced(true);
            break;
        }
    }
}

void PageReplacement::openDiskFile(string diskFileName)
{
    this->diskFile.open(diskFileName.c_str());
}

void PageReplacement::closeDiskFile()
{
    this->diskFile.close();
}

void PageReplacement::writePageToDisk(int removedPageIndex)
{
    int i = 0;
    diskFile << "Removed Physical Page Index : [" << removedPageIndex << "]" << endl;
    for (; i < this->pm->getFrameSize(); ++i)
        diskFile << this->pm->getPages()[removedPageIndex][i] << ", ";
    diskFile << endl << endl;
    this->pm->setValidAt(removedPageIndex, true);
}