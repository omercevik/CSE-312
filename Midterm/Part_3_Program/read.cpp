/*
    CSE 312
    Operating Systems
    Midterm Project
    read.cpp
    Omer CEVIK
    161044004
*/
#include "FileSystem.h"

void FileSystem::readCommand(string pathDir, string pathFile)
{
    if (pathDir[0] != '/')
    {
        cerr << "Error: Root path of directory is not available!" << endl;
        exit(EXIT_FAILURE);
    }

    fstream file(pathFile,ios::out);
    if (!file.is_open())
    {
        cerr << "Error: Can not open file! " << pathFile << endl;
        exit(EXIT_FAILURE);
    }

    int i = getDataBlock(1) + 2;
    int lengthOfFile = -1;
    int dbAddress = -1;
    int iNodeAddress = -1;
    string directoryId = "";
    bool flag = false;

    vector<string> pathVector = split(pathDir,'/');
    vector<string> dataBlockVector, dataBlockPath;

    while(inputFileVector.at(i) != "}")
    {
        dataBlockVector = split(inputFileVector.at(i), ' ');
        string dbFilePath = dataBlockVector[1];

        if (dbFilePath.compare(pathDir) == 0)
        {
            lengthOfFile = stoi(dataBlockVector[3]);
            dbAddress = stoi(dataBlockVector[5]);
            flag = true;
            break;
        }
        ++i;
    }
    if (!flag)
    {
        cerr << "Error: Invalid File Path! " << pathDir << endl;
        exit(EXIT_FAILURE);
    }

    if (lengthOfFile < blockSize)
    {
        string outFile = "";
        int j;
        for (j = getDataBlock(dbAddress)+ 5; inputFileVector.at(j+1).compare("\n") != 0 && inputFileVector.at(j+2).compare("EOF") != 0; ++j)
            outFile += inputFileVector.at(j) + "\n";
        outFile += inputFileVector.at(j);
        file << outFile;
    }
    else
    {
        int blocks = lengthOfFile/blockSize == lengthOfFile*1.0/blockSize ? lengthOfFile/blockSize : lengthOfFile/blockSize + 1;
        int indexStart = getDataBlock(dbAddress)+5;
        int indexEnd = getDataBlock(dbAddress+blocks);
        int tempDbAddress = dbAddress+1;
        string outFile = "";
        bool flag = true;
        for (int i = 0; (i < blocks) && flag; ++i, ++tempDbAddress)
        {
            string dataBlock = "Data-Block ["+ to_string(tempDbAddress) +"]";

            while(inputFileVector.at(indexStart+1).compare(dataBlock) != 0)
            {
                if (inputFileVector.at(indexStart+2).compare("EOF") == 0)
                {
                    outFile += inputFileVector.at(indexStart);
                    flag = false;
                    break;
                }
                if (inputFileVector.at(indexStart+2).compare(dataBlock) == 0)
                    outFile += inputFileVector.at(indexStart);
                else
                    outFile += inputFileVector.at(indexStart) + "\n";

                ++indexStart;
            }

            indexStart += 3;
        }
        file << outFile;
    }

    file.close();
}