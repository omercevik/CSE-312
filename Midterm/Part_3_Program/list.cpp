/*
    CSE 312
    Operating Systems
    Midterm Project
    list.cpp
    Omer CEVIK
    161044004
*/
#include "FileSystem.h"

void FileSystem::listCommand(string path)
{
    if (path[0] != '/')
    {
        cerr << "Error: Root path is not available!" << endl;
        exit(EXIT_FAILURE);
    }

    vector<string> pathVector = split(path, '/');
    set<string> listDirs, listFiles;
    string rwe = "rw-rw-rw-\t";

    int i = getDataBlock(0) + 2;

    while(inputFileVector.at(i) != "}")
    {
        vector<string> dataBlockVector = split(inputFileVector.at(i),' ');
        vector<string> dataBlockPath = split(dataBlockVector[2], '/');

        int j = 0;
        for (j = 0; j < pathVector.size() && j < dataBlockPath.size() && (pathVector[j].compare(dataBlockPath[j]) == 0); ++j);
        if (j == pathVector.size() && j < dataBlockPath.size())
            listDirs.insert(dataBlockPath[j]);
        ++i;
    }

    i = getDataBlock(1) + 2;
    int totalBytes = 0;

    while(inputFileVector.at(i) != "}")
    {
        vector<string> dataBlockVector = split(inputFileVector.at(i),' ');
        vector<string> dataBlockPath = split(dataBlockVector[1], '/');

        int j = 0;
        for (j = 0; j < pathVector.size() && j < dataBlockPath.size() && (pathVector[j].compare(dataBlockPath[j]) == 0); ++j);
        if (j == pathVector.size() && j < dataBlockPath.size())
        {
            for (set<string>::iterator it1 = listDirs.begin(); it1 != listDirs.end();  ++it1)
            {
                if (it1->compare(dataBlockPath[j]) != 0)
                {
                    listFiles.insert(dataBlockVector[3] + "\t" + dataBlockPath[j]);
                    totalBytes += stoi(dataBlockVector[3]);
                    break;
                }
            }
        }
        ++i;
    }

    cout << "total " << totalBytes << endl;

    for (set<string>::iterator it1 = listDirs.begin(); it1 != listDirs.end();  ++it1)
        cout << "d" << rwe  << "\t" << *it1 << endl;

    for (set<string>::iterator it1 = listFiles.begin(); it1 != listFiles.end();  ++it1)
        cout << "-" << rwe  << *it1 << endl;
}