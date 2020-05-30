/*
    CSE 312
    Operating Systems
    Midterm Project
    part2.cpp
    Omer CEVIK
    161044004
*/

#include <iostream>
#include <string>
#include <fstream>

using namespace std;

void getArguments(char const *argv[], int* blockSize, int* numberOfFreeINodes, string* fileSystem);
void createFileSystem(string fileName, int iNodeSize, int blockSize);

int main(int argc, char const *argv[])
{
    if (argc != 4)
    {
        cerr << "Unacceptable arguments!" << endl;
        cerr << "Usage : ./makeFileSystem 4 400 mySystem.dat" << endl;
        exit(-1);
    }

    int blockSize = 0, numberOfFreeINodes = 0;
    string fileSystem = "";

    getArguments(argv, &blockSize, &numberOfFreeINodes, &fileSystem);

    createFileSystem(fileSystem,numberOfFreeINodes,blockSize);

    return 0;
}

void createFileSystem(string fileName, int iNodeSize, int blockSize)
{
    ofstream file(fileName);
    if (!file.is_open())
    {
        cerr << "Can not open the \"" << fileName << "\" file!" << endl;
        exit(1);
    }

    for (int i = 0; i < 1024*1024; ++i, file.flush())
        file << endl;

    file.seekp(ios::beg);

    double totalDataBlocks = 0, totalINodeBlocks = 0;
    int totalDataBlockSizeKB = 0, totalINodeBlockSizeKB = 0;

    totalINodeBlocks = iNodeSize * 40.0 / 1024 / blockSize;

    totalINodeBlocks = totalINodeBlocks > (int)totalINodeBlocks ? (int)(totalINodeBlocks+1) : totalINodeBlocks;

    totalINodeBlockSizeKB = blockSize * totalINodeBlocks;

    totalDataBlockSizeKB = 1024 - totalINodeBlockSizeKB - 1;

    totalDataBlocks = totalDataBlockSizeKB / blockSize;

    file << "SUPER BLOCK" << endl
             << "{" << endl
             << "\tMagic Number: 1," << endl
             << "\tBlocks: " << totalDataBlocks << "," << endl
             << "\tI Node Blocks: " << totalINodeBlocks << "," << endl
             << "\tI Nodes: " << iNodeSize << endl
             << "}" << endl;

    for (int i = 0; i < iNodeSize; ++i)
    {
        file << "I-NODE [" << i << "]" << endl
             << "{" << endl
             << "\tvalid: 1," << endl
             << "\tsize: "<< blockSize << " KB," << endl
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

    for (int i = 0; i < totalDataBlocks; ++i)
    {
        file << "Data-Block [" << i << "]" << endl
             << "{" << endl
             << "}" << endl;
    }

    file.close();
}

void getArguments(char const *argv[], int* blockSize, int* numberOfFreeINodes, string* fileSystem)
{
    sscanf(argv[1],"%d",blockSize);
    sscanf(argv[2],"%d",numberOfFreeINodes);
    string temp(argv[3]);
    *fileSystem = temp;

    if (*blockSize < 1)
    {
        cerr << "Unacceptable block size!" << endl;
        exit(-1);
    }
    if (*numberOfFreeINodes < 3)
    {
        cerr << "Unacceptable free i-node size!" << endl;
        exit(-1);
    }
    if (fileSystem == NULL || fileSystem->compare("") == 0)
    {
        cerr << "Unacceptable file system name!" << endl;
        exit(-1);
    }
}