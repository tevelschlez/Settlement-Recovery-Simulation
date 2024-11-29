# Please implement your Makefile rules and targets below.
# Customize this file to define how to build your project.
	
all: clean compile link cleanObjAndRun

compile:
	gcc -g -Wall -Weffc++ -std=c++11 -c -Iinclude src/*.cpp

Link:
	g++ -g -Wall -Weffc++ -std=c++11 -o bin/simulation *.o

clean:
	rm -f *.o
	
cleanObjAndRun:
	rm -f *.o
	clear
