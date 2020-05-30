/*
    CSE 312
    Operating Systems
    Midterm Project
    part3.cpp
    Omer CEVIK
    161044004
*/
#include "FileSystem.h"

void getArguments(int argc, char const *argv[], string* fileName, string* command, string* parameter1, string* parameter2);

int main(int argc, char const *argv[])
{
    string inputFile = "", command = "", parameter1 = "", parameter2 = "";

    getArguments(argc, argv, &inputFile, &command, &parameter1, &parameter2);

    FileSystem fsystem(inputFile);

    fsystem.executeCommand(command, parameter1, parameter2);

    return 0;
}


void getArguments(int argc, char const *argv[], string* fileName, string* command, string* parameter1, string* parameter2)
{
    if (argc > 2)
    {
        string temp(argv[1]);
        *fileName = temp;
        string temp2(argv[2]);
        *command = temp2;

        if (argc == 4 && (temp2.compare("mkdir") == 0 || temp2.compare("rmdir") == 0 || temp2.compare("list") == 0 || temp2.compare("del") == 0))
        {
            string temp3(argv[3]);
            *parameter1 = temp3;
            *parameter2 = "";
        }
        else if (argc == 5 && (temp2.compare("write") == 0 || temp2.compare("read") == 0))
        {
            string temp3(argv[3]);
            *parameter1 = temp3;
            string temp4(argv[4]);
            *parameter2 = temp4;
        }
        else if (argc == 3 && (temp2.compare("dumpe2fs") == 0))
        {
            *parameter1 = "";
            *parameter2 = "";
        }
        else
        {
            cerr << "Error: Unknown command!" << endl;
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        cerr << "Error: Unaccepted parameters!" << endl;
        exit(EXIT_FAILURE);
    }
}