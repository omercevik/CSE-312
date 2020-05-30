/*
    CSE 312
    Operating Systems
    Midterm Project
    del.cpp
    Omer CEVIK
    161044004
*/
#include "FileSystem.h"

void FileSystem::delCommand(string path)
{
    if (path[0] != '/')
    {
        cerr << "Error: Root path of directory is not available!" << endl;
        exit(EXIT_FAILURE);
    }

    int i = getDataBlock(1) + 2;
    int lengthOfFile = -1;
    int dbAddress = -1;
    int iNodeAddress = -1;
    bool flag = false;

    vector<string> pathVector = split(path,'/');
    vector<string> dataBlockVector;

    while(inputFileVector.at(i) != "}")
    {
        dataBlockVector = split(inputFileVector.at(i), ' ');
        string dbFilePath = dataBlockVector[1];

        if (dbFilePath.compare(path) == 0)
        {
            lengthOfFile = stoi(dataBlockVector[3]);
            iNodeAddress = stoi(dataBlockVector[4]);
            dbAddress = stoi(dataBlockVector[5]);
            flag = true;
            break;
        }
        ++i;
    }
    if (!flag)
    {
        cerr << "Error: Invalid File Path! " << path << endl;
        exit(EXIT_FAILURE);
    }

    int totalDelete = 0;

    if (lengthOfFile < blockSize)
    {
        while (totalDelete < lengthOfFile)
        {
            totalDelete += inputFileVector.at(getDataBlock(dbAddress)+ 2).size();
            inputFileVector.erase(inputFileVector.begin() + getDataBlock(dbAddress)+ 2);
        }
        inputFileVector.erase(inputFileVector.begin() + getDataBlock(dbAddress)+ 2);
    }
    else
    {
        int blocks = lengthOfFile/blockSize == lengthOfFile*1.0/blockSize ? lengthOfFile/blockSize : lengthOfFile/blockSize + 1;
        int indexStart = getDataBlock(dbAddress);
        int indexEnd = getDataBlock(dbAddress+blocks);
        int tempDbAddress = dbAddress;
        inputFileVector.erase(inputFileVector.begin() + indexStart,inputFileVector.begin() + indexEnd );
        for (int i = 0; i < blocks; ++i, ++tempDbAddress, ++indexStart)
        {
            string dataBlock = "Data-Block ["+ to_string(tempDbAddress) +"]\n{\n}";
            inputFileVector.insert(inputFileVector.begin() + indexStart, dataBlock);
        }
    }
    inputFileVector.erase(inputFileVector.begin() + i);
    setDirectBlockOfINodeFree(iNodeAddress, dbAddress);
}