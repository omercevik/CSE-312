/*
    CSE 312
    Operating Systems
    Midterm Project
    FileSystem.h
    Omer CEVIK
    161044004
*/
#ifndef FILE_SYSTEM_H
#define FILE_SYSTEM_H

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>
#include <sstream>
#include <iterator>
#include <set>
#include <ctime>
#include <cstdlib>

using namespace std;

class FileSystem
{
public:
    FileSystem(){}
    FileSystem(string fileName);
    vector<string> split(string path, char c);
    void readFileSystem(string fileName);
    string readFile(string fileName);
    void executeCommand(string command, string parameter1, string parameter2);
    void getInitValues();
    void getSuperBlockValues(string line, int* value);
    bool isINodeValid(int index);
    int isDirectBlockOfINodeFree(int indexINode);
    bool isDirectBlockFree(int indexDirectBlock);
    void setDataBlockAddressInINode(int index, int dbAddress);
    void setDataBlockContent(int dbAddress, string content);
    int getDataBlock(int index);
    int getEmptyDataBlock();
    int getVirtualDataBlockAddress(string address);
    int containsDirPath(int indexINode, string path);
    void setDirectBlockOfINodeFree(int indexINode, int dbAddress);
    int getINodeDataBlockAddress(int iNodeAddress);
    void writeToFile();
    void createEmptyFileSystem();

    void mkdirCommand(string path);
    void rmdirCommand(string path);
    void listCommand(string path);
    void writeCommand(string pathDir, string pathFile);
    void readCommand(string pathDir, string pathFile);
    void delCommand(string path);
    void dumpe2fsCommand();

private:
    string fileSystemName;
    int blockSize;
    int totalBlocks;
    int totalINodes;
    int totalINodeBlocks;
    int iNodeBegin;
    int dataBlockBegin;
    int directoryCounter = 0;
    vector<string> inputFileVector;
};

#endif