#include "stdafx.h"
#include "fileAccess.h"

/**************************************************************
*	  Purpose:  Constructs fileAccess object, sets fileName to 
*				the file name in the parameters.
*
*         Entry:  Takes a string file name.
*
*          Exit:  Returns a fileAccess object.
****************************************************************/
fileAccess::fileAccess(std::string inFileName)
{
	fileName = inFileName;	
}

/**************************************************************
*	  Purpose:  Default deconstructor.
*
*         Entry:  NA
*
*          Exit:  NA
****************************************************************/
fileAccess::~fileAccess()
{
}

/**************************************************************
*	  Purpose:  Get the size of the file indicated by the fileName
*				in bytes.
*
*         Entry:  Uses the object's fileName value to identify the 
*					file - must be a valid file name or the 
*					method will return -1 as error.
*
*          Exit:  NA
****************************************************************/
long fileAccess::getOutFileSize()
{
	outFile.open(fileName, std::ios::app);
	if (!outFile)
	{
		std::cout << "We have an error opening file" << std::endl;
		return -1;
	}
	outFile.seekp(0, std::ios::end);
	outSize = outFile.tellp();
	outFile.close();
	return outSize;
}

/**************************************************************
*	  Purpose:  Reads the file pointed to by the stream object
*				and reads in the file bytes starting at the position
*				indicated by the position value in the parameters and
*				ending after reading in the number of bytes indicated
*				by the length value in the parameters. Stores these byte
*				values as char values in the buffer in the parameters.
*
*         Entry:  Method requires a valid fstream object with an attached
*					file, and a valid char buffer object with the correct 
*					array length.
*
*          Exit:  Updates the buffer object in the parameter by adding
*					the requested file char values into it. Returns true
*					if the method was successful.
****************************************************************/
bool fileAccess::getData(std::fstream& stream, long position, int length, char buffer[])
{
	if (getOutFileSize() < (position + long(length)))
	{
		std::cout << "reading from file not allowed due to file size overrun or file does not exist";
		return false;
	}
	else
	{
		stream.open(fileName, std::ios::in | std::ios::ate);
		if (!stream)
		{
			std::cout << "error opening file" << std::endl;
			return false;
		}
		stream.seekg(position, std::ios::beg);
		stream.read(buffer, length);
		stream.close();

		return true;
	}
}

/**************************************************************
*	  Purpose:  Reads in an entire file as char values in a 
*				buffer array.
*
*         Entry:  Uses fileAccess object stream value and the 
*					getData method.
*
*          Exit:  Creates and modifies a char vector buffer.
****************************************************************/
std::vector<char> fileAccess::readEntireFile()
{
	std::vector<char> entireFile;

	if (getOutFileSize() > -1)
	{
		char* buffer = NULL;
		buffer = new char[outSize];
		getData(rFile, 0l, outSize, buffer);

		for (int i = 0; i < outSize; i++)
		{
			entireFile.push_back(buffer[i]);
		}

		delete buffer;
	}
	else
	{
		std::cout << "We were not able to  access file" << std::endl;
	}

	return entireFile;
}