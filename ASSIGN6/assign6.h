/*********************************************************************
PROGRAM:    CSCI 480 Assignment 6
PROGRAMMER: 
LOGON ID:  
DUE DATE:  
FUNCTION:   Simulation loaded into memory or a process requests a block
            of memory dynamically process and allocate dealoocate and
            tremina bolck using Best-fit and  First-fit
*********************************************************************/
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <limits>
#include <string>
#include <list>


using namespace std;

#ifndef ASSIGN6_H
#define ASSIGN6_H

#define HOWOFTEN 5
#define MB 1048576
#define KB 1024


class MemoryBlock{
    public:
	MemoryBlock(int,int,string,string);
        MemoryBlock(int,int );
        int GetAdd();
        int GetSize();
        string GetPID();
        string GetBID();
        void SetAdd(int);
        void SetSize(int);
        void SetPID(string);
        void SetBID(string);
        void Print();

    private:
       int StaringAddress = 0;
       int Size = 0;
       string pID;
       string BlockID;

};

list<MemoryBlock*> inUse;
list<MemoryBlock*> Avail;


void LoadTran(string, int, string, int);
void AllocateTran(list<MemoryBlock>*, string, int, string);
void DeallocateTran(string, string, string);
void TerminateTran(string);
void PrintInfo(list<MemoryBlock*>);
void Attach(MemoryBlock*);

MemoryBlock* SearchProcess(string, string);
list<MemoryBlock*>::iterator BestFit(int);
list<MemoryBlock*>::iterator FirstFit(int);

#endif
