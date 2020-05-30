/*
    CSE 312
    Operating Systems
    Midterm Project
    dumpe2fs.cpp
    Omer CEVIK
    161044004
*/
#include "FileSystem.h"

void FileSystem::dumpe2fsCommand()
{
    cout << "Super Block :" << endl
         << "\tBlocks : " << totalBlocks << endl
         << "\tI-Nodes : " << totalINodes << endl
         << "\tI-Node Blocks : " << totalINodeBlocks << endl
         << "\tI-Node Begin : " <<  iNodeBegin << endl
         << "\tData Block Begin : " <<  dataBlockBegin << endl;

    vector<int> emptyDBAddresses, usedDBAddresses, emptyINodeAddresses, usedINodeAddresses;

    for (int i = 0; i < totalINodes; ++i)
    {
        int index = isDirectBlockOfINodeFree(i);
        if (index == 0)
            emptyINodeAddresses.push_back(i);
        else
        {
            usedINodeAddresses.push_back(i);
            if (index == -1)
                usedINodeAddresses.push_back(0);
            else
                usedINodeAddresses.push_back(index);
        }
    }

    cout << "Empty I Node Size : " << emptyINodeAddresses.size() << endl
         << "Empty I Nodes : " << endl << "\t";

    for (int i = 0; i < emptyINodeAddresses.size(); ++i)
        cout << emptyINodeAddresses.at(i) << ", ";

    cout << endl;

    cout << "Used I Node Size : " << usedINodeAddresses.size()/2 << endl
         << "Used I Nodes : " << endl;

    for (int i = 0; i < usedINodeAddresses.size()-1; i+=2)
        cout << "\t" << usedINodeAddresses.at(i) << " : " << usedINodeAddresses.at(i+1) << " I Nodes used, " << endl;

    for (int i = 0; i < totalBlocks; ++i)
    {
        int index = isDirectBlockFree(i);
        if (index == 0)
            usedDBAddresses.push_back(i);
        else
            emptyDBAddresses.push_back(i);
    }

    cout << "Empty Data Block Size : " << emptyDBAddresses.size() << endl
         << "Empty Data Blocks : " << endl << "\t";

    for (int i = 0; i < emptyDBAddresses.size(); ++i)
        cout << emptyDBAddresses.at(i) << ", ";

    cout << endl;

    cout << "Used Data Block Size : " << usedDBAddresses.size() << endl
         << "Used Data Blocks : " << endl << "\t";

    for (int i = 0; i < usedDBAddresses.size(); ++i)
        cout << usedDBAddresses.at(i) << ", ";

    cout << endl;

    cout << "Directories :" << endl;

    for (int i = getDataBlock(0) + 2; inputFileVector.at(i) != "}"; ++i)
        cout << "\t" << inputFileVector.at(i) << endl;

    cout << "Files :" << endl;

    for (int i = getDataBlock(1) + 2; inputFileVector.at(i) != "}"; ++i)
        cout << "\t" << inputFileVector.at(i) << endl;

}