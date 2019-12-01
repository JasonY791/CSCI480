#
# PROGRAM:    Assignment 7
# PROGRAMMER:
# LOGON ID:   
# DATE DUE:   
#

# Compiler variables
CCFLAGS = -ansi -Wall -std=c++11 -g

#Rule to link object code files to create executable file
Assign7: Assign7.o
	g++ $(CCFLAGS) -o Assign7 Assign7.o
#Rule to compile source code file to object code
Assign7.o: Assign7.cpp
	g++ $(CCFLAGS) -c Assign7.cpp
#Pseudo-target to remove object code and executable files
clean:
	-rm *.o Assign7
