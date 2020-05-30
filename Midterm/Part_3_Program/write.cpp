/*
    CSE 312
    Operating Systems
    Midterm Project
    write.cpp
    Omer CEVIK
    161044004
*/
#include "FileSystem.h"

void FileSystem::writeCommand(string pathDir, string pathFile)
{
    if (pathDir[0] != '/')
    {
        cerr << "Error: Root path of directory is not available!" << endl;
        exit(EXIT_FAILURE);
    }
    string fileString = readFile(pathFile);

    int i = getDataBlock(0) + 2, j = getEmptyDataBlock(), dbId = -1;
    string directoryId = "";
    time_t now = time(0);
    char* dt = ctime(&now);
    string timeString = "";
    while(*dt != '\n')
        timeString += *dt++;
    bool flag = false;

    vector<string> pathVector = split(pathDir,'/');
    vector<string> dataBlockVector, dataBlockPath;

    while(inputFileVector.at(i) != "}")
    {
        dataBlockVector = split(inputFileVector.at(i), ' ');
        dataBlockPath = split(dataBlockVector[2], '/');

        int k = 0;
        for (k = 0; k < pathVector.size()-1; ++k)
            if (pathVector.at(k).compare(dataBlockPath.at(k)) != 0)
            {
                directoryId = dataBlockVector[0];
                break;
            }
        if (k == pathVector.size()-1)
        {
            string pathId = "";
            string pathTemp = "";
            for (int i = 0; i < pathVector.size()-2; ++i)
                pathTemp += pathVector[i] + "/";
            pathTemp += pathVector[pathVector.size()-2];
            if (pathVector.size() == 2)
                pathTemp = "/";
            for (int i = getDataBlock(0) + 2; inputFileVector.at(i) != "}"; ++i)
            {
                dataBlockVector = split(inputFileVector.at(i), ' ');
                if (dataBlockVector.at(2).compare(pathTemp) == 0)
                {
                    pathId = dataBlockVector.at(0);
                    break;
                }
            }

            string insertLine = pathDir + " " + pathFile + " " + timeString + "\n" + to_string(fileString.size()-1) + "\n";
            string tempLine = "";
            if (fileString.size() > blockSize-1)
            {
                inputFileVector.insert(inputFileVector.begin() + j + 2, insertLine);
                int dataBlockSize = fileString.size()/blockSize;
                int fileCounter = 0;
                for (int i = 0; i < dataBlockSize; ++i)
                {
                    for (int k = 0; k < blockSize; ++k)
                        tempLine += fileString[fileCounter++];

                    if (isDirectBlockFree(1))
                    {
                        dbId = getVirtualDataBlockAddress(inputFileVector.at(j));

                        setDataBlockAddressInINode(stoi(pathId), dbId);
                        setDataBlockContent(1,"0 " + pathDir + " " + pathFile + " " + to_string(fileString.size()-1) + " " + pathId + " " + to_string(dbId) + " " + timeString);
                        j += 2;
                    }
                    else
                    {
                        int index = getDataBlock(1) + 2;
                        string id = split(inputFileVector.at(index),' ').at(0);
                        index = stoi(id) + 1;
                        dbId = getVirtualDataBlockAddress(inputFileVector.at(j));
                        setDataBlockAddressInINode(stoi(pathId), dbId);
                        setDataBlockContent(1,to_string(index) + " " + pathDir + " " + pathFile + " " + to_string(fileString.size()-1) + " " + pathId + " " + to_string(dbId) + " " + timeString);
                        j += 2;
                    }

                    inputFileVector.insert(inputFileVector.begin() + j + 2, tempLine);
                    j = getDataBlock(dbId+1);

                    if (j == 0)
                    {
                        cerr << "There is not enough empty data block!" << endl;
                        exit(EXIT_FAILURE);
                    }
                    tempLine = "";
                }
                if (fileCounter < fileString.size())
                {
                    for (int i = fileCounter; i < fileString.size(); ++i)
                        tempLine += fileString[fileCounter++];
                    tempLine += "\nEOF\n";
                    j = getDataBlock(dbId+1);
                    inputFileVector.insert(inputFileVector.begin() + j + 2, tempLine);
                }
            }
            else
            {
                inputFileVector.insert(inputFileVector.begin() + j + 2, insertLine);

                if (isDirectBlockFree(1))
                {
                    int dbId = getVirtualDataBlockAddress(inputFileVector.at(j));
                    setDataBlockAddressInINode(stoi(pathId), dbId);
                    setDataBlockContent(1,"0 " + pathDir + " " + pathFile + " " + to_string(fileString.size()-1) + " " + pathId + " " + to_string(dbId) + " " + timeString);
                }
                else
                {
                    int index = getDataBlock(1) + 2;
                    string id = split(inputFileVector.at(index),' ').at(0);
                    index = stoi(id) + 1;
                    int dbId = getVirtualDataBlockAddress(inputFileVector.at(j));
                    setDataBlockAddressInINode(stoi(pathId), dbId);
                    setDataBlockContent(1,to_string(index) + " " + pathDir + " " + pathFile + " " + to_string(fileString.size()-1) + " " + pathId + " " + to_string(dbId) + " " + timeString);
                }
                fileString += "\nEOF\n";
                inputFileVector.insert(inputFileVector.begin() + j + 4, fileString);
            }
            flag = true;
            break;
        }
        ++i;
    }

    if (!flag)
        cerr << "Error: Invalid Directory Path! " << pathDir << endl;
}