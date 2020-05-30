/*
    CSE 312
    Operating Systems
    Midterm Project
    rmdir.cpp
    Omer CEVIK
    161044004
*/
#include "FileSystem.h"

void FileSystem::rmdirCommand(string path)
{
    if (path[0] != '/')
    {
        cerr << "Error: Root path is not available!" << endl;
        exit(EXIT_FAILURE);
    }

    vector<string> pathVector = split(path, '/');
    vector<string> dataBlockVector, dataBlockPath;

    int dirIndex = getDataBlock(0) + 2, fileIndex = getDataBlock(1) + 2;

    if (path.compare("/") == 0)
    {
        createEmptyFileSystem();
    }
    else
    {
        bool flag = true;

        while(inputFileVector.at(dirIndex) != "}")
        {
            dataBlockVector = split(inputFileVector.at(dirIndex), ' ');
            string dbPath = dataBlockVector[2];
            if (dbPath.compare(path) == 0)
            {
                flag = false;
                break;
            }
            ++dirIndex;
        }

        if (flag)
        {
            cerr << "Error: There is no directory in path! " << path << endl;
            exit(EXIT_FAILURE);
        }

        while(inputFileVector.at(fileIndex) != "}")
        {
            dataBlockVector = split(inputFileVector.at(fileIndex), ' ');

            dataBlockPath = split(dataBlockVector[1], '/');

            int j = 0;
            for (j = 0; j < pathVector.size() && j < dataBlockPath.size() && (pathVector[j].compare(dataBlockPath[j]) == 0); ++j);
            if (j == pathVector.size())
            {
                delCommand(dataBlockVector[1]);
            }
            else
                ++fileIndex;
        }

        dirIndex = getDataBlock(0) + 2;

        while(inputFileVector.at(dirIndex) != "}")
        {
            dataBlockVector = split(inputFileVector.at(dirIndex), ' ');
            string dbPath = dataBlockVector[2];
            if (dbPath.compare(path) == 0)
            {
                inputFileVector.erase(inputFileVector.begin() + dirIndex);
            }
            else
                ++dirIndex;
        }
        writeToFile();
    }
}

void FileSystem::createEmptyFileSystem()
{
    ofstream file(fileSystemName, ios::out);
    if (!file.is_open())
    {
        cerr << "Can not open the \"" << fileSystemName << "\" file!" << endl;
        exit(1);
    }

    for (int i = 0; i < 1024*1024; ++i, file.flush())
        file << endl;

    file.seekp(ios::beg);


    file << "SUPER BLOCK" << endl
             << "{" << endl
             << "\tMagic Number: 1," << endl
             << "\tBlocks: " << totalBlocks << "," << endl
             << "\tI Node Blocks: " << totalINodeBlocks << "," << endl
             << "\tI Nodes: " << totalINodes << endl
             << "}" << endl;

    for (int i = 0; i < totalINodes; ++i)
    {
        if (i == 0)
        {
            file << "I-NODE [" << i << "]" << endl
             << "{" << endl
             << "\tvalid: 1," << endl
             << "\tsize: " << blockSize/1024 << " KB," << endl
             << "\tdirect block 0: 0," << endl
             << "\tdirect block 1: 1," << endl
             << "\tdirect block 2: NA," << endl
             << "\tdirect block 3: NA," << endl
             << "\tdirect block 4: NA," << endl
             << "\tindirect single block: NA," << endl
             << "\tindirect double block: NA," << endl
             << "\tindirect triple block: NA" << endl
             << "}" << endl;
        }
        else
        {
            file << "I-NODE [" << i << "]" << endl
                 << "{" << endl
                 << "\tvalid: 1," << endl
                 << "\tsize: " << blockSize/1024 << " KB," << endl
                 << "\tdirect block 0: NA," << endl
                 << "\tdirect block 1: NA," << endl
                 << "\tdirect block 2: NA," << endl
                 << "\tdirect block 3: NA," << endl
                 << "\tdirect block 4: NA," << endl
                 << "\tindirect single block: NA," << endl
                 << "\tindirect double block: NA," << endl
                 << "\tindirect triple block: NA" << endl
                 << "}" << endl;
        }
    }

    for (int i = 0; i < totalBlocks; ++i)
    {
        if (i == 0)
        {
            file << "Data-Block [" << i << "]" << endl
                 << "{" << endl
                 << "0 0 /" << endl
                 << "}" << endl;
        }
        else
        {
            file << "Data-Block [" << i << "]" << endl
                 << "{" << endl
                 << "}" << endl;
        }
    }

    file.close();
}