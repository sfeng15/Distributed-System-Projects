COMPILER = g++
COMPILER_FLAGS =-c -std=c++0x -g -O0 
LINKER = g++

all:  chord_main 

chord_main :  chord_main_working.o string_spliter.o
	$(LINKER) chord_main_working.o string_spliter.o  -pthread -o mychord 

chord_main_working.o: chord_main_working.cpp spliter.h 
	$(COMPILER) $(COMPILER_FLAGS) chord_main_working.cpp
string_spliter.o: string_spliter.cpp spliter.h
	$(COMPILER) $(COMPILER_FLAGS) string_spliter.cpp


clean:
	-rm  -f *.o mychord 
