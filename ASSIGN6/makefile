#MAKE FILE
#
#PROGRAM:    Assignment 6
#PROGRAMMER: 
#ZID:        
#DATE DUE:   
#


#Compiler variables
CC = g++
CCFLAGS = -Wall -std=c++11


#Rule to link object code files to create executable file
Assign6: Assign6.o
	$(CC) $(CCFLAGS) -o Assign6 Assign6.o


#Rules to compile source code files to object code	
Assign6.o: Assign6.cpp
	$(CC) $(CCFLAGS) -c Assign6.cpp


#Pseudo-target to remove object code and executable files	
clean:
	-rm *.o Assign6
