CC = g++
CFLAGS = -c
VALG = valgrind -v

all:part3 run

part3: part3.o fileSystem.o mkdir.o rmdir.o write.o read.o list.o del.o dumpe2fs.o
	$(CC) part3.o fileSystem.o mkdir.o rmdir.o write.o read.o list.o del.o dumpe2fs.o -o fileSystemOper

part3.o: part3.cpp
	$(CC) $(CFLAGS) part3.cpp

fileSystem.o: fileSystem.cpp
	$(CC) $(CFLAGS) fileSystem.cpp

mkdir.o: mkdir.cpp
	$(CC) $(CFLAGS) mkdir.cpp

rmdir.o: rmdir.cpp
	$(CC) $(CFLAGS) rmdir.cpp

write.o: write.cpp
	$(CC) $(CFLAGS) write.cpp

read.o: read.cpp
	$(CC) $(CFLAGS) read.cpp

list.o: list.cpp
	$(CC) $(CFLAGS) list.cpp

del.o: del.cpp
	$(CC) $(CFLAGS) del.cpp

dumpe2fs.o: dumpe2fs.cpp
	$(CC) $(CFLAGS) dumpe2fs.cpp

run:
	./fileSystemOper fileSystem.data mkdir "/usr"
	./fileSystemOper fileSystem.data mkdir "/usr/ysa"
	./fileSystemOper fileSystem.data mkdir "/bin/ysa"
	./fileSystemOper fileSystem.data write "/usr/ysa/file1" fileSystem.cpp
	./fileSystemOper fileSystem.data write "/usr/file2" Makefile
	./fileSystemOper fileSystem.data write "/file3" Makefile
	./fileSystemOper fileSystem.data list "/"
	./fileSystemOper fileSystem.data del "/usr/ysa/file1"
	./fileSystemOper fileSystem.data dumpe2fs
	./fileSystemOper fileSystem.data read "/usr/file2" linuxFile2.data
	cmp linuxFile2.data Makefile
	./fileSystemOper fileSystem.data list "/usr"
	./fileSystemOper fileSystem.data write "/usr/linkedfile2" Makefile
	./fileSystemOper fileSystem.data dumpe2fs
	./fileSystemOper fileSystem.data list "/usr"
	./fileSystemOper fileSystem.data del "/usr/file2"
	./fileSystemOper fileSystem.data list "/usr"
	./fileSystemOper fileSystem.data rmdir "/"
	./fileSystemOper fileSystem.data dumpe2fs
	./fileSystemOper fileSystem.data list "/"

v:part3 runv

runv:
	$(VALG) ./fileSystemOper fileSystem.data mkdir "/usr"

clean:
	rm -rf *o fileSystemOper