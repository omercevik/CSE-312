CC = g++
CFLAGS = -c -std=c++11
VALG = valgrind -v
PTHREAD = -pthread

all:part2

part2: part2.o virtualMemory.o physicalMemory.o sorter.o pageTable.o pageReplacement.o
	$(CC) part2.o virtualMemory.o physicalMemory.o sorter.o pageTable.o pageReplacement.o -o sortArrays $(PTHREAD) -std=c++11

part2.o: part2.cpp
	$(CC) $(CFLAGS) part2.cpp

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
	./sortArrays 10 4 7 LRU local 10000 diskFileName.dat

v:part2 runv

runv:
	$(VALG) ./sortArrays 12 5 10 LRU local 10000 diskFileName.dat


clean:
	rm -rf *o sortArrays