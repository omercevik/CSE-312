CC = g++
CFLAGS = -c
VALG = valgrind -v

all:part2 run

part2: part2.o
	$(CC) part2.o -o makeFileSystem

part2.o: part2.cpp
	$(CC) $(CFLAGS) part2.cpp

run:
	./makeFileSystem 4 400 fileSystem.data

v:part2 runv

runv:
	$(VALG) ./makeFileSystem 4 400 fileSystem.data


clean:
	rm -rf *o makeFileSystem fileSystem.data