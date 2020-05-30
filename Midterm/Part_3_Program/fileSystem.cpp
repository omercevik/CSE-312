/*
    CSE 312
    Operating Systems
    Midterm Project
    fileSystem.cpp
    Omer CEVIK
    161044004
*/
#include "FileSystem.h"

void FileSystem::executeCommand(string command, string parameter1, string parameter2)
{
    if (inputFileVector.at(2).find("1") != std::string::npos)
    {
        if (containsDirPath(0,"/") == -1)
        {
            setDataBlockAddressInINode(0,0);
            setDataBlockAddressInINode(0,1);
            setDataBlockContent(0,"0 0 /");
        }

        if (command.compare("mkdir") == 0)
        {
            mkdirCommand(parameter1);
            writeToFile();
        }
        else if (command.compare("rmdir") == 0)
        {
            rmdirCommand(parameter1);
        }
        else if (command.compare("list") == 0)
        {
            listCommand(parameter1);
            writeToFile();
        }
        else if (command.compare("write") == 0)
        {
            writeCommand(parameter1,parameter2);
            writeToFile();
        }
        else if (command.compare("read") == 0)
        {
            readCommand(parameter1,parameter2);
            writeToFile();
        }
        else if (command.compare("del") == 0)
        {
            delCommand(parameter1);
            writeToFile();
        }
        else if (command.compare("dumpe2fs") == 0)
        {
            dumpe2fsCommand();
            writeToFile();
        }
        else
        {
            cerr << "Error: Command is not available! " << command << endl;
            exit(EXIT_FAILURE);
        }
    }
}

void FileSystem::writeToFile()
{
    fstream file(fileSystemName,ios::out);
    if (file.is_open())
    {
        for (int i = 0; i < inputFileVector.size(); ++i)
            file << inputFileVector[i] << endl;
        file.close();
    }
}

int FileSystem::containsDirPath(int indexINode, string path)
{
    int index = -1;
    vector<string> dataBlockVector;

    for (int i = getDataBlock(indexINode)+2; inputFileVector.at(i) != "}"; ++i)
    {
        dataBlockVector = split(inputFileVector.at(i), ' ');
        if (path.compare(dataBlockVector[2]) == 0)
        {
            index = i;
            break;
        }
    }
    return index;
}

int FileSystem::getVirtualDataBlockAddress(string address)
{
    string id = "";
    for (int i = 12; address[i] != ']'; ++i)
        id += address[i];
    return stoi(id);
}

int FileSystem::getEmptyDataBlock()
{
    int index = -1;
    for (int i = 2; i < totalBlocks; ++i)
        if ((index = getDataBlock(i)) != -1)
        {
            if (inputFileVector.at(index+2).compare("}") == 0)
                break;
            else
                index = -1;
        }

    return index;
}

vector<string> FileSystem::split(string path, char c)
{
    vector<string> tokens;
    string token;
    istringstream tokenStream(path);

    while (std::getline(tokenStream, token, c))
        tokens.push_back(token);

    return tokens;
}

string FileSystem::readFile(string filePath)
{
    fstream file(filePath,ios::in);
    if (file.is_open())
    {
        string fileString = "";
        string line = "";
        while(getline(file, line))
            fileString += line + "\n";
        file.close();

        return fileString;
    }
    else
    {
        cerr << "Error: Can not open file! " << filePath << endl;
        exit(EXIT_FAILURE);
    }
}

FileSystem::FileSystem(string fileName)
{
    fileSystemName = fileName;
    readFileSystem(fileName);
    getInitValues();
}

int FileSystem::getDataBlock(int index)
{
    string directAddress = "Data-Block [" + to_string(index) +"]";
    int i = 0;

    for (i = dataBlockBegin; i < inputFileVector.size(); ++i)
        if (inputFileVector.at(i).compare(directAddress) == 0)
            break;
    return i == inputFileVector.size() ? -1 : i;
}

void FileSystem::setDataBlockContent(int dbAddress, string content)
{
    int ithDirectBlock = getDataBlock(dbAddress);
    inputFileVector.insert(inputFileVector.begin() + ithDirectBlock+2, content);
}

void FileSystem::setDataBlockAddressInINode(int index, int dbAddress)
{
    if (isINodeValid(index))
    {
        int id = isDirectBlockOfINodeFree(index);

        int ithDB = (index*13) + iNodeBegin + 4 + id;

        inputFileVector[ithDB] = "\tdirect block " + to_string(id) + ": " + to_string(dbAddress) + ",";
    }
}

bool FileSystem::isDirectBlockFree(int indexDirectBlock)
{
    int indexDB = getDataBlock(indexDirectBlock);

    return indexDB == -1 ? false : inputFileVector.at(indexDB+2).compare("}") == 0;
}

int FileSystem::isDirectBlockOfINodeFree(int indexINode)
{
    int ithINode = (indexINode*13) + iNodeBegin + 4;
    for (int i = 0; i < 5; ++i, ++ithINode)
        if (inputFileVector.at(ithINode).find("NA") != std::string::npos)
            return i;
    return -1;
}

void FileSystem::setDirectBlockOfINodeFree(int indexINode, int dbAddress)
{
    int ithINode = (indexINode*13) + iNodeBegin + 4;
    for (int i = 0; i < 5; ++i, ++ithINode)
    {
        int dbInt = getINodeDataBlockAddress(ithINode);
        if (dbInt == dbAddress)
        {
            for (int j = i; j < 4; ++j)
            {
                int dbInt = getINodeDataBlockAddress(ithINode+j+1);
                inputFileVector[ithINode+j] = dbInt == -1 ? "\tdirect block " + to_string(j) + ": NA," : "\tdirect block " + to_string(j) + ": " + to_string(dbInt) + ",";
            }
            break;
        }
    }
}

int FileSystem::getINodeDataBlockAddress(int iNodeAddress)
{
    string db = inputFileVector.at(iNodeAddress);
    string dbString = "";
    for (int j = 17; db[j] != ','; ++j)
        dbString += db[j];

    return dbString.compare("NA") == 0 ? -1 : stoi(dbString);
}

bool FileSystem::isINodeValid(int index)
{
    int ithINode = (index*13) + iNodeBegin;
    return inputFileVector.at(ithINode+2).find("1") != std::string::npos;
}

void FileSystem::getInitValues()
{
    getSuperBlockValues(inputFileVector.at(3),&totalBlocks);
    getSuperBlockValues(inputFileVector.at(4),&totalINodeBlocks);
    getSuperBlockValues(inputFileVector.at(5),&totalINodes);

    string blockSizeString = inputFileVector.at(10);
    string toIntBlockSizeString = "";
    for (int i = 7; blockSizeString[i] != ' '; ++i)
        toIntBlockSizeString += blockSizeString[i];

    blockSize = stoi(toIntBlockSizeString) * 1024;

    std::vector<string>::iterator itr = std::find(inputFileVector.begin(), inputFileVector.end(), "I-NODE [0]");
    iNodeBegin = std::distance(inputFileVector.begin(), itr);

    itr = std::find(inputFileVector.begin(), inputFileVector.end(), "Data-Block [0]");
    dataBlockBegin = std::distance(inputFileVector.begin(), itr);
}

void FileSystem::getSuperBlockValues(string line, int* value)
{
    stringstream ss;
    ss << line;
    string temp;
    while (!ss.eof())
    {
        ss >> temp;
        if (stringstream(temp) >> *value)
            break;
        temp = "";
    }
}

void FileSystem::readFileSystem(string fileName)
{
    fstream file(fileName,ios::in);
    if (file.is_open())
    {
        string line;
        while(getline(file, line))
            inputFileVector.push_back(line);
        file.close();
    }
}