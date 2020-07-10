CC = g++
CFLAGS = -c -std=c++11
VALG = valgrind -v
PTHREAD = -pthread

all:part3

part3: part3.o virtualMemory.o physicalMemory.o sorter.o pageTable.o pageReplacement.o
	$(CC) part3.o virtualMemory.o physicalMemory.o sorter.o pageTable.o pageReplacement.o -o sortArrays $(PTHREAD) -std=c++11

part3.o: part3.cpp
	$(CC) $(CFLAGS) part3.cpp

virtualMemory.o: virtualMemory.cpp
	$(CC) $(CFLAGS) virtualMemory.cpp

physicalMemory.o: physicalMemory.cpp
	$(CC) $(CFLAGS) physicalMemory.cpp

sorter.o: sorter.cpp
	$(CC) $(CFLAGS) sorter.cpp

pageTable.o: pageTable.cpp
	$(CC) $(CFLAGS) pageTable.cpp

pageReplacement.o: pageReplacement.cpp
	$(CC) $(CFLAGS) pageReplacement.cpp

run:
	./sortArrays LRU local diskFileName.dat

v:part3 runv

runv:
	$(VALG) ./sortArrays LRU local diskFileName.dat


clean:
	rm -rf *o sortArrays