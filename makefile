# Please implement your Makefile rules and targets below.
# Customize this file to define how to build your project.

all: clean compile link cleanObjAndRun

compile:
	gcc -g -Wall -Weffc++ -std=c++11 -c -Iinclude src/*.cpp

link:
	g++ -g -Wall -Weffc++ -std=c++11 -o bin/simulation *.o

clean:
	rm -f *.o
	
cleanObjAndRun:
	rm -f *.o
	clear

valgrind: all
	valgrind --leak-check=full --show-reachable=yes ./bin/simulation config_file.txt

	
	
	