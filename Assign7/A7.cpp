/*********************************************************************
PROGRAM:    CSCI 480 Assignment 7
PROGRAMMER: 
LOGON ID:  
DUE DATE:   
FUNCTION:   This assignment involves maintaining a File Allocation Table.
			We will simulate "FAT-12" on a (very small) disk. Each block
			will be 512 bytes in size. With FAT-12, we have 4096 blocks,
			so the disk is only 2 MB in size.
			Starting with an empty root directory, we will carry out a
			series of transactions. From time to time we will print the
			contents of the directory and (part) of the FAT.

*********************************************************************/
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

//define constant variables
const int HOWOFTEN = 5;
const int NUMOFBYTES = 512;
const int MAXIMUM = 12;
const int NUMTOPRINT = 240;
const int DISKSIZE = 4096;

//define a class that stores all the entries
class File{
	private:
		int size;
		string fileName;
		vector<int>blocks;
	public:
		File(int filesize, string name);
		File();
		int getSize();
		string getName();
		vector<int> getBlocks();
		void setSize(int newSize);
		void setName(string newName);
		void insertBlocks(int newBlock);

};

/***********************************************************************

Function: File(int filesize, string name)

Use: constuctor

Parameters: 1.int filesize
            2.string name

Returns:  None

***********************************************************************/
File::File(int filesize, string name)
{
	size = filesize;
	fileName = name;
}



/***********************************************************************

Function: File()

Use: constuctor

Parameters: None

Returns:  None

***********************************************************************/
File::File()
{

}



/***********************************************************************

Function: setSize(int newSize)

Use: setter

Parameters: 1.int newSize

Returns:  None

***********************************************************************/
void File::setSize(int newSize)
{
	size = newSize;
}



/***********************************************************************

Function: setName(string newName)

Use: setter

Parameters: 1.string newName

Returns:  None

***********************************************************************/
void File::setName(string newName)
{
	fileName = newName;
}



/***********************************************************************

Function: insertBlocks(int newBlock)

Use: setter

Parameters: 1.int newBlock

Returns:  None

***********************************************************************/
void File::insertBlocks(int newBlock)
{
	blocks.push_back(newBlock);
}



/***********************************************************************

Function:getSize()

Use: getter

Parameters: None

Returns:file size
***********************************************************************/
int File::getSize()
{
	return size;
}



/***********************************************************************

Function:getName()

Use: getter

Parameters: None

Returns:file name
***********************************************************************/
string File::getName()
{
	return fileName;
}



/***********************************************************************

Function:getBlocks()

Use: getter

Parameters: None

Returns:file blocks
***********************************************************************/
vector<int> File::getBlocks()
{
	return blocks;
}



//function prototypes
void Copy(string name,string newName);
void Delete(string name, int isCalledByMain);
void Create(string name,int size, int isCalledByMain);
void Modify(string name, int newSize);
void Rename(string oldName, string newName, int isCalledByMain);
void ModifyRoot();
void print();

//define global variables
vector<int> disk(DISKSIZE);
vector<File> files;


int main(int argc, char *argv[]) {
	//initialize the directory to be empty
	for(unsigned int i=0; i < disk.size(); i++) {
		disk[i] = 0;
	}

	//open the file
	ifstream infile;
	infile.open("data.txt");
	istringstream inStream;
	string readingline;
	string firstletter;
	string name;
	string newname;
	int size;
	int counter = 0;
	//create the first two files
	Create(".",NUMOFBYTES, 0);
	Create("..",0, 0);

	cout << "Beginning of the FAT simulation"<<endl;
	while(!infile.eof())
	{
		//read one line each time
		getline(infile,readingline);
		inStream.str(readingline);
		//read in first character
		inStream>>firstletter;

		// print table after every 5 entries
		if(counter%HOWOFTEN == 0)
			print();

		//if the first character is ?, end the table
		if(firstletter =="?")
		{
			cout<<endl<<"End of the FAT simulation"<<endl;
			break;
		}
		//if the first character is C, copy the file
		else if(firstletter == "C")
		{
			cout<<"Transaction:  Copy a file"<<endl;
			inStream>>name;
			inStream>>newname;
			Copy(name,newname);
		}
		//if the first character is D, delete the file
		else if(firstletter == "D")
		{
			cout<<"Transaction:  Delete a file"<<endl;
			inStream>>name;
			Delete(name, 1);
		}
		//if the first character is N, create the file
		else if(firstletter == "N")
		{
			cout<<"Transaction:  Add a new file"<<endl;
			inStream>>name;
			inStream>>size;
			Create(name,size,1);
		}
		//if the first character is M, modify the file
		else if(firstletter =="M")
		{
			cout<<"Transaction:  Modify a file"<<endl;
			inStream>>name;
			inStream>>size;
			Modify(name, size);
		}
		//if the first character is R, rename the file
		else if(firstletter =="R")
		{
			cout<<"Transaction:  Rename a file"<<endl;
			inStream>>name;
			inStream>>newname;
			Rename(name, newname,1);
		}
		counter ++;

		//a directory block can contain up to 12 entries, if it is
		//over 12, add increase one block
		int oldBlocks =	files.size() / MAXIMUM;
		if(files.size() % MAXIMUM >0)
		{
			oldBlocks++;
		}
		//call ModifyRoot method to increase blocks
		if(files[0].getSize()<oldBlocks*NUMOFBYTES)
		{
			files[0].setSize(oldBlocks*NUMOFBYTES);
			ModifyRoot();
		}
	}
	//close the file
	infile.close();
	//print the table again
	print();

	return 0;
}

/***********************************************************************

Function: ModifyRoot()

Use: Increase block

Parameters: None

Returns:None
***********************************************************************/
void ModifyRoot()
{
	for(unsigned int j=0 ; j<disk.size();j++)
	{
		//find a empty block
		if(disk[j]==0)
		{
			//assign it with the block index it links to
			int size = files[0].getBlocks().size();
			int value = files[0].getBlocks().at( size-1 );
			disk[value] = j;
			files[0].insertBlocks(j);
			disk[j] = -1;
			return;
		}
	}
}

/***********************************************************************

Function: Create(string name,int size, int isCalledByMain)

Use: Create a new file

Parameters: 	1.string name
		2.int size
		3.int isCalledByMain

Returns:None
***********************************************************************/
void Create(string name,int size, int isCalledByMain)
{
	//check for file existence
	for(unsigned int i=0; i < files.size(); i++)
	{
		if(files[i].getName() == name)
		{
			cerr<<"Error! The file name, "<<name<< ", already exists";
			return;
		}
	}
	//create a file object for the new file
	File file(size, name);

	//check for the size
	int numOfBlocks = size/NUMOFBYTES;
	if(size % NUMOFBYTES > 0 )
		numOfBlocks++;

	vector<int>track;
	int count =0;
	while(count != numOfBlocks )
	{
		for(unsigned int j=0 ; j<disk.size();j++)
	  	{
			//find the available blocks and store it
			if(disk[j]==0)
			{
				track.push_back(j);
				file.insertBlocks(j);
				count++;
			}
			if(count == numOfBlocks)
			{
				break;
			}
	  	}
	}
	track.push_back(-1);
	for(unsigned int i=0;i<track.size()-1;i++)
	{
		int z = track[i];
		disk[z]=track[i+1];
	}
	if(isCalledByMain == 1)
		cout << "Successfuly added a new file, " <<name<<", of size " <<size<<endl;
	files.push_back(file);

}



/***********************************************************************

Function: Delete(string name, int isCalledByMain)

Use: Delete an existing file

Parameters: 1.string name
	    2.int isCalledByMain

Returns:None
***********************************************************************/
void Delete(string name, int isCalledByMain)
{
	bool fileExist  = false;
	int index = 0;
	//check the file existence
	for(unsigned int i = 0; i < files.size(); i++){
		if(name == files[i].getName()){
			fileExist = true;
			index = i;
			break;
		}
	}
	if(fileExist == false)
	{
		cout<<"Error!  The file name, "+name+ ", was not found."<<endl;
		return;
	}
	else
	{
		//delete the file from the directory and clean the blocks
		for(unsigned int j = 0; j < files[index].getBlocks().size(); j++)
			disk.at(files[index].getBlocks().at(j) )= 0;
		files.erase(files.begin()+index);
		if(isCalledByMain == 1)
		cout << "Successfully deleted a file, " +name<<endl;
	}

}

/***********************************************************************

Function: Copy(string name, string newName)

Use: copy a file

Parameters: 	1.string name
		2.string newNam

Returns:None
***********************************************************************/
void Copy(string name, string newName)
{
	bool old = false;
	int size = 0;
	//check the file existence
	for(unsigned int i = 0; i < files.size(); i++)
	{
		if(files[i].getName() == name)
		{
			old = true;
			size = files[i].getSize();
			break;
		}
	}
	if(old == false)
	{
		cout<<"Error!  The old file name, "+ name +", was not found."<<endl;
		return;
	}
	for(unsigned int j = 0; j < files.size(); j++)
	{
		if(files[j].getName() == newName)
		{
			cout<<"Error!  The new file name, "+ newName +", already exists."<<endl;
			return;
		}
	}
	//copy the file
	Create(newName, size, 0);
	cout<<"Successfully copied an existing file, "+name+", to a new file, "+newName<<endl;

}



/***********************************************************************

Function: Rename(string oldName, string newName,int isCalledByMain)

Use: rename a file

Parameters: 	1.string oldName
		2.string newName
		3.int isCalledByMain

Returns:None
***********************************************************************/
void Rename(string oldName, string newName,int isCalledByMain)
{
	bool old = false;
	int index;
	//check the file existence
	for(unsigned int i = 0; i < files.size(); i++)
	{
		if(files[i].getName() == oldName)
		{
			old = true;
			index = i;
			break;
		}
	}
	if(old == false)
	{
		cout<<"Error!  The old file name, "+ oldName +", was not found."<<endl;
		return;
	}
	for(unsigned int j = 0; j < files.size(); j++)
	{
		if(files[j].getName() == newName)
		{
			cout<<"Error!  The new file name, "+ newName +", already exists."<<endl;
			return;
		}
	}
	//rename the file
	files[index].setName(newName);
	if(isCalledByMain == 1)
		cout << "Successfully changed the file name "+oldName+" to " +newName<<endl;
}



/***********************************************************************

Function: Modify(string name, int newSize)

Use: modify the size of a file

Parameters: 	1.string name
		2.int newSize

Returns:None
***********************************************************************/
void Modify(string name, int newSize)
{
	bool old = false;
	//check the file existence
	for(unsigned int i = 0; i < files.size(); i++)
	{
		if(files[i].getName() == name)
		{
			old = true;
			break;
		}
	}
	if(old == false)
	{
		cout<<"Error!  The file name, "+ name+ ", was not found."<<endl;
		return;
	}
	else
	{
		//copy the old file, delete it and rename it back
		Create(name+"(copy)",newSize, 0);
		Delete(name, 0);
		Rename(name+"(copy)",name, 0);
		cout << "Successfully modified a file, " + name<<endl;
	}
}



/***********************************************************************

Function: print()

Use: Print the directory

Parameters: None

Returns:None
***********************************************************************/
void print()
{
	int totalbytes = 0;
	cout <<endl<< "Directory Listing"<<endl;
	for(unsigned int i =0; i < files.size(); i++)
	{
		cout<<"File name: "<<setw(23)<<left<<files[i].getName();
		cout<<"File Size: "<<setw(6)<<right<<files[i].getSize()<<endl;
		cout<<"Cluster(s) in use:";
		if( files[i].getBlocks().size() == 0)
		{
			cout<< "  (none)"<<endl;
		}
		else
		{
			int counter = 0;
			for(unsigned int j=0; j < files[i].getBlocks().size(); j++)
			{
				cout<< setw(6) << right <<files[i].getBlocks().at(j);
				counter++;
				if(counter%MAXIMUM==0&&counter/MAXIMUM !=0)
					cout<<endl<<"                  ";
			}
			cout<<endl;
		}
		totalbytes += files[i].getSize();
	}
	cout<<"Files: "<<files.size()<<"  Total Size: "<<totalbytes<<" bytes"<<endl;
	cout<<endl<<"Contents of the File Allocation Table"<<endl;
	int counter = 0;
	for(int i =0; i < NUMTOPRINT; i++)
	{
		if(counter % MAXIMUM == 0)
		{
			cout<< "#" <<setw(3)<<setfill('0') << i <<"-"<<setw(3)<<setfill('0')<<i+MAXIMUM-1<<" ";
		}
		cout<<right<<setfill(' ')<<setw(6)<<disk[i];
		counter++;
		if(counter % MAXIMUM == 0)
		{
			cout<<endl;
			counter=0;
		}
	}
	cout<<endl;
}
