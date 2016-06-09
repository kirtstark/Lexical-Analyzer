/************************************************************************
* Class: fileAccess
*
* Constructors:
*	fileAccess(std::string fileName)
*		outSize is initialized to 0.
*
* Mutators:
*	NA
*
* Methods:
*   long getOutFileSize()
*		This method returns the size of the file stored on the disk.
*   bool getData(std::fstream& stream, long position, int length, char buffer[])
*		This method uses the given stream object to load the data characters from the file into the buffer.
*   std::vector<char> readEntireFile()
*		This method reads the entire file into a char vector and returns that vector.
*************************************************************************/

#pragma once
#include "stdafx.h"
#include <fstream>
#include <string>
#include <iostream>
#include <vector>


class fileAccess
{
public:
	// constructor that initialized the fileName value using the parameter
	fileAccess(std::string fileName);

	// default deconstructor
	~fileAccess();

	// returns the current size of the file on the disk
	long getOutFileSize();

	// reads in char characters from the disk file
	// from the starting position in the paraemter
	// for the length in the paramter
	// and stores that data in the buffer
	// returns true if successful, false if the method fails
	bool getData(std::fstream& stream, long position, int length, char buffer[]);

	// reads in the entire file from the disk
	// and returns it as a char vector
	std::vector<char> readEntireFile();

	std::ifstream inFile;
	std::ofstream outFile;
	std::fstream rFile;

private:
	std::string fileName;
	long outSize;
};

