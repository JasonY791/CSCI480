/*********************************************************************
PROGRAM:    CSCI 480 Assignment 6
PROGRAMMER: 
LOGON ID:   
DUE DATE:  
FUNCTION:   Simulation loaded into memory or a process requests a block
            of memory dynamically process and allocate dealoocate and
            tremina bolck using Best-fit and  First-fit
*********************************************************************/
#include"Assign6.h"




int main(int argc, char**argv){

//judge amount of arguments
  if(argc != 2){
     cerr<<"There are should be 2 arguments"<<endl;
     exit(-1);
  }

//initial the avail list
  MemoryBlock *bl1 = new MemoryBlock(3* MB,MB);
  Avail.push_back(bl1);
  MemoryBlock *bl2 = new MemoryBlock(4* MB,2*MB);
  Avail.push_back(bl2);
  MemoryBlock *bl3 = new MemoryBlock(6* MB,2*MB);
  Avail.push_back(bl3);
  MemoryBlock *bl4 = new MemoryBlock(8* MB,4*MB);
  Avail.push_back(bl4);
  MemoryBlock *bl5 = new MemoryBlock(12* MB,4*MB);
  Avail.push_back(bl5);



  string argument = argv[1];
  int way=0;
//use bestfir
  if(argument == "B" || argument =="b"){
     way = 1;
     cout<<"Simulation of Memory Management using the Best-Fit algorithm"<<endl<<endl;
     cout<<endl<<"Beginning of the run "<<endl<<endl;
     cout<<"List of available blocks"<<endl;
     PrintInfo(Avail);
     cout<<endl<<"List of blocks in use "<<endl;
     PrintInfo(inUse);

  }
// fisrt fit
  else if(argument == "F" || argument =="f"){
     way = 2;
     cout<<"Simulation of Memory Management using the First-Fit algorithm"<<endl<<endl;
     cout<<endl<<"Beginning of the run "<<endl<<endl;
     cout<<"List of available blocks"<<endl;
     PrintInfo(Avail);
     cout<<endl<<"List of blocks in use "<<endl;
     PrintInfo(inUse);

  }
//no that choice
  else{
     cout<<"no that choice";
  }

//deal with file
  ifstream infile;
  infile.open("data6.txt");

  if(infile.fail()){
     cerr << "fail to open it" << endl;
     exit(-1);
  }

  string readingline;

//reading
  while(getline(infile,readingline)){

//when read "?"
    if(readingline[0] == '?'){
       cout<<endl<<endl<<"End of the run "<<endl<<endl;
       cout<<"List of available blocks"<<endl;
       PrintInfo(Avail);
       cout<<endl<<"List of blocks in use "<<endl;
       PrintInfo(inUse);
       break;
    }

    string firstletter;
    string ProcessName;
    string ProcessID;
    int size;


    istringstream inStream(readingline);

// reading "L"    load
    inStream >> firstletter;
    if(firstletter == "L"){
        inStream>> ProcessID;
        inStream>> size;
        inStream>> ProcessName;
        cout<<endl<<"Transaction:  request to load process "<<ProcessID<<", block ID "<<ProcessName<<" using "<<size<<" bytes"<<endl;
        LoadTran(ProcessID, size, ProcessName, way);
    }

// reading A   allocate
    else if(firstletter == "A"){
        inStream>> ProcessID;
        inStream>> size;
        inStream>> ProcessName;
        cout<<endl<<"Transaction:  request to allocate "<<size<<" bytes for process "<<ProcessID<<", block ID: "<<ProcessName<<endl;
        LoadTran(ProcessID, size, ProcessName, way);

    }

//reading D dellocate
    else if(firstletter == "D"){
        inStream>>ProcessID;
        inStream>>ProcessName;
        cout<<endl<<"Transaction:  request to deallocate block ID "<<ProcessName<<" for process "<<ProcessID<<endl;
        DeallocateTran(ProcessID, ProcessName, firstletter);
    }

//"T"   terminate
   else{
        inStream>>ProcessID;
        cout<<endl<<"Transaction:  request to terminate process "<<ProcessID<<endl;
        TerminateTran(ProcessID);
   }

//print two lists
   cout<<endl<<"List of available blocks"<<endl;
   PrintInfo(Avail);
   cout<<endl<<"List of blocks in use"<<endl;
   PrintInfo(inUse);


 }


 return 0;
}




/***********************************************************************

Function: MemoryBlock(int startadd, int size)

Use: constuctor

Parameters: 1.int startadd
            2.int size

Returns:  None

***********************************************************************/
MemoryBlock::MemoryBlock(int startadd, int size){
    SetAdd(startadd);
    SetSize(size);
}





/***********************************************************************

Function: MemoryBlock(int startadd, int size, string pid, string bid)

Use: constuctor

Parameters: 1.int startadd
            2.int size
            3.string pid
            4.string bid

Returns:  None

***********************************************************************/
MemoryBlock::MemoryBlock(int startadd, int size, string pid, string bid){
    SetAdd(startadd);
    SetSize(size);
    SetPID(pid);
    SetBID(bid);
}





/***********************************************************************

Function: GetAdd()

Use: get address

Parameters: None

Returns:  starting address

***********************************************************************/
int MemoryBlock::GetAdd(){
    return StaringAddress;
}






/***********************************************************************

Function:GetSize()

Use: get size

Parameters: None

Returns:  size

***********************************************************************/
int MemoryBlock::GetSize(){
   return Size;
}





/***********************************************************************

Function:GetPID()

Use: get process id

Parameters: None

Returns: process id
***********************************************************************/
string MemoryBlock::GetPID(){
   return pID;
}






/***********************************************************************

Function:GetBID()

Use: get block id

Parameters: None

Returns: block id
***********************************************************************/
string MemoryBlock::GetBID(){
   return BlockID;
}






/***********************************************************************

Function:SetAdd()

Use: setting start address

Parameters: 1.int startadd

Returns:None
***********************************************************************/
void MemoryBlock::SetAdd(int startadd){
    StaringAddress = startadd;
}





/***********************************************************************

Function:SetSize()

Use: setting start address

Parameters: 1.int startadd

Returns:None
***********************************************************************/
void MemoryBlock::SetSize(int size){
   Size = size;
}





/***********************************************************************

Function:SetPID()

Use: setting process id

Parameters: 1.string pid

Returns:None
***********************************************************************/
void MemoryBlock::SetPID(string pid){
  pID = pid;
}






/***********************************************************************

Function:SetBID()

Use: setting block id

Parameters: 1.string bid

Returns:None
***********************************************************************/
void MemoryBlock::SetBID(string bid){
  BlockID = bid;
}






/***********************************************************************

Function:Print()

Use: print the private members

Parameters:None

Returns:None
***********************************************************************/
void MemoryBlock::Print(){
  cout<<"Start Address = "<<setw(8)<<GetAdd()<<" Size = " <<setw(8) <<GetSize()<<"  Process ID = "<<GetPID()<<"  Block ID = "<<GetBID()<<endl;
}




/***********************************************************************

Function: AllocateTran( list<MemoryBlock*> ::iterator it, string id, int size, string name)

Use: Allocate transaction

Parameters: 1.list<MemoryBlock*> ::iterator it,
            2.string id
            3.int size,
            4.string name

Returns:None
***********************************************************************/
void AllocateTran( list<MemoryBlock*> ::iterator it, string id, int size, string name){
  MemoryBlock *current = *it;
  MemoryBlock *b1 = new MemoryBlock(current->GetAdd(),size,id,name);
  int begin = current->GetAdd()+size;

  inUse.push_front(b1);
  Avail.erase(it++);

  int remainingBlock = current->GetSize()-size;
  if(remainingBlock >0){
      MemoryBlock *memoryBlock2 = new MemoryBlock(begin, remainingBlock, "","" );
      Avail.insert(it, memoryBlock2);
  }

}






/***********************************************************************

Function: void LoadTran(string id, int size, string name, int way)

Use: Allocate transaction

Parameters: 1.string id
            2.int size
            3.string name
            4.int way

Returns:None
***********************************************************************/
void LoadTran(string id, int size, string name, int way)
{
  list<MemoryBlock *>::iterator it;
// first fit
  if(way == 2 ){
     it = FirstFit(size);
     if(it == Avail.end()){
         cerr<<"Unable to comply as no block of adequate size is available"<<endl;
         return;
     }
  }

//best fit
  else if(way ==1){
          it = BestFit(size);
	  if(it == Avail.end()){
             cout<<"Unable to comply as no block of adequate size is available" << endl;
             return;
          }
          else{
             if((*it)->GetSize() < size){
                 cout<<"Unable to comply as no block of adequate size is available"<<endl;
             }
             else{
                 cout<<"Found a block of size "<< (*it)->GetSize()<< endl<<"Success in allocating a block"<<endl;
             }
          }
  }
  AllocateTran(it,id,size,name);

}





/***********************************************************************

Function: void DeallocateTran(string id, string name, string transaction)

Use: Deallocate the transaction

Parameters: 1.string id
            2.string name
            3.string transaction
Returns: None
***********************************************************************/
void DeallocateTran(string id, string name, string transaction){
  MemoryBlock *bl = SearchProcess(id ,name);

//null pointer
  if(bl == nullptr){
     cout<<"Unable to comply as the indicated block cannot be found." << endl;
     return;
  }

//attch
  Attach(bl);
  MemoryBlock * current = *(Avail.begin());
  int endAddress = current->GetAdd()+current->GetSize();
  int startAddress;

  list<MemoryBlock*>::iterator it = Avail.begin();
  it++;

  while(it !=Avail.end()){
    current = *it;
    startAddress = current->GetAdd();
     // if equal
    if(endAddress == startAddress){
       int blockSize = current->GetSize();
       it--;

       MemoryBlock *lastBlock = *it;
       int newSize = lastBlock->GetSize() + blockSize;

       // merging
       if(newSize < 4 *MB){
          cout<<"Merging two blocks at "<< setw(8) << lastBlock->GetAdd()<<"  and  "<<setw(8)<< current->GetAdd()<<endl;
          MemoryBlock *newBlock = new MemoryBlock(lastBlock->GetAdd(), newSize);

          Avail.erase(it++);
          delete (lastBlock);
          lastBlock = *it;
          Avail.erase(it++);
          delete (lastBlock);
          lastBlock = nullptr;
          //insert
          Avail.insert(it, newBlock);

         //endaddress
          endAddress = newBlock->GetAdd() + newBlock->GetSize();
          continue;
       }
       else{
          it++;
          endAddress = startAddress + blockSize;
          continue;
       }
    }

    endAddress = current->GetSize() + startAddress;
    it++;
  }
  if(transaction == "D"){
     cout<<"Success in deallocating a block"<<endl;
  }

}








/***********************************************************************

Function: void TerminateTran(string id)

Use: terminate the transaction

Parameters: 1.string id

Returns:None
***********************************************************************/
void TerminateTran(string id){
  list<MemoryBlock*>::iterator it = inUse.begin();
  bool process = false;

  while(it != inUse.end()){
     MemoryBlock *current = *it;
     if(current->GetPID() == id){
        process = true;
	it++;

         //call deallocate
        DeallocateTran(id, current->GetBID(), "T");
     }
     else{
        it++;
     }
  }

  if(process){
     cout<< "Success in terminating a process " << endl;
  }
  else{
     cout << "Unable to comply as the indicated process could not be found." << endl;
  }
}






/***********************************************************************

Function: PrintInfo(list<MemoryBlock*> newList)

Use: to print the information of block

Parameters: 1.list<MemoryBlock*> newList

Returns:None
***********************************************************************/
void PrintInfo(list<MemoryBlock*> nlist)
{
  int listSize = 0;
  //not empty and in avail
  if(!nlist.empty() && nlist != inUse)
  {
     for(list<MemoryBlock *>::iterator it = nlist.begin(); it != nlist.end(); it++){
	 MemoryBlock *bl = *it;
         listSize += bl->GetSize();
	 cout<<"Start Address = "<<setw(8)<< bl ->GetAdd()<<"  Size = "<<setw(8)<< bl->GetSize()<<endl;
     }
  }
  // not empty and in inUse
  else if(!nlist.empty() && nlist == inUse){
     for(list<MemoryBlock *>::iterator it = nlist.begin(); it != nlist.end(); it++){
         MemoryBlock *bl = *it;
	 listSize += bl->GetSize();
         bl->Print();
     }
  }
  else{
     cout<< "(none)"<<endl;
  }
  cout << "Total size of the list = " << listSize << endl;


}






/***********************************************************************

Function: Attach(MemoryBlock *bl)

Use: attach the block

Parameters: 1.MemoryBlock *bl

Returns:None
***********************************************************************/
void Attach(MemoryBlock *bl){
  for(list<MemoryBlock*>::iterator it = Avail.begin(); it != Avail.end(); it++){
      MemoryBlock *current = *it;
      if(current->GetAdd() > bl->GetAdd()){
	 Avail.insert(it, bl);
         return;
      }
  }
}






/***********************************************************************

Function: MemoryBlock* SearchProcess(string id, string name)

Use: search the process

Parameters: 1.string id
            2.string name

Returns: the process which is looked for or none
***********************************************************************/
MemoryBlock* SearchProcess(string id, string name)
{
  for(list<MemoryBlock *>::iterator it = inUse.begin(); it != inUse.end(); it++){
      MemoryBlock *bl = *it;
      if((bl->GetPID() == id )&&( bl->GetBID() == name)){
          inUse.erase(it);
          return bl;
      }
  }
  return nullptr;
}







/***********************************************************************

Function: list<MemoryBlock*>::iterator FirstFit(int size)

Use: Traversing the list for fisrtFit

Parameters: 1.int size

Returns: pointer
***********************************************************************/
list<MemoryBlock*>::iterator FirstFit(int size)
{
  list<MemoryBlock*>::iterator it = Avail.begin();
  while(it != Avail.end()){
     MemoryBlock *bl = *it;
     if(bl->GetSize() == size || bl->GetSize() > size){
        cout<<"Found a block of size "<< bl->GetSize()<<endl<<"Success in allocating a block " <<endl;
        return it;
     }
     it++;
   }
   return it;
}







/***********************************************************************

Function: list<MemoryBlock*>::iterator BestFit(int size)

Use: Traversing the list for bestFit

Parameters: 1.int size

Returns: pointer
***********************************************************************/
list<MemoryBlock*>::iterator BestFit(int size)
{
  list<MemoryBlock*>::iterator it = Avail.begin();
  list<MemoryBlock*>::iterator endIt = Avail.end();

  int sizeLimit = numeric_limits<int>::max();

  while(it != Avail.end()){
     MemoryBlock *bl = *it;
     int blocksize = bl->GetSize();
     if(bl->GetSize() == size || bl->GetSize() > size){
         if(blocksize < sizeLimit){
            endIt = it;
            sizeLimit = (*endIt)->GetSize();
         }
     }
     it++;
  }

  return endIt;
}
