/*
    CSE 312
    Operating Systems
    Midterm Project
    mkdir.cpp
    Omer CEVIK
    161044004
*/
#include "FileSystem.h"

void FileSystem::mkdirCommand(string path)
{
    if (path[0] != '/')
    {
        cerr << "Error: Root path is not available!" << endl;
        exit(EXIT_FAILURE);
    }

    if (containsDirPath(0,path) != -1)
    {
        cerr << "Error: There is already directory in that path " << path << endl;
        exit(EXIT_FAILURE);
    }

    vector<string> pathVector = split(path, '/');
    time_t now = time(0);
    char* dt = ctime(&now);
    string timeString = "";
    while(*dt != '\n')
        timeString += *dt++;

    int i = getDataBlock(0) + 2, directoryId = 0;
    vector<string> dataBlockVector = split(inputFileVector.at(i), ' ');
    int directoryLength = stoi(dataBlockVector[0]) + 1;

    if (pathVector.size() == 2)
    {
        dataBlockVector = split(inputFileVector.at(i), ' ');
        directoryId = stoi(dataBlockVector[0]) + 1;
        string directoryPath = to_string(directoryId) + " 0 " + path + " " + timeString;
        directoryLength = 1;
        setDataBlockContent(0, directoryPath);
    }
    else if (pathVector.size() > 2)
    {
        while(inputFileVector.at(i) != "}")
        {
            dataBlockVector = split(inputFileVector.at(i), ' ');

            vector<string> dataBlockPath = split(dataBlockVector[2], '/');

            --directoryLength;
            int j = 0;
            for (j = 0; j < pathVector.size() && j < dataBlockPath.size() && (pathVector[j].compare(dataBlockPath[j]) == 0); ++j);
            if (j == dataBlockPath.size())
            {
                string previousDirId = dataBlockVector[0];
                i = getDataBlock(0) + 2;
                dataBlockVector = split(inputFileVector.at(i), ' ');

                directoryId = stoi(dataBlockVector[0]) + 1;
                string directoryPath = to_string(directoryId) + " " + previousDirId + " " + path + " " + timeString;
                setDataBlockContent(0, directoryPath);
                break;
            }
            ++i;
        }
    }
    if (directoryLength == 0)
    {
        i = getDataBlock(0) + 2;
        inputFileVector.erase(inputFileVector.begin() + i);
        cerr << "Error : Incorrect path to mkdir! " << path  << endl;
    }
}