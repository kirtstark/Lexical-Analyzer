/************************************************************************
* Class: cFileData
*
* Constructors:
*	cFileData()
*	cFileData(std::string name);
*
* Mutators:
*	NA
*
* Methods:
*   void display()
*		This method prints the object to the screen, varName and value
*************************************************************************/

#pragma once
#include <vector>
#include <string>
#include "cToken.h"
#include "cSymbol.h"
#include "cSymbolTable.h"

class cFileData
{
public:
	cFileData();
	cFileData(std::string name);
	~cFileData();

	std::string fileName;
	std::vector<cToken> parsedFile;
	std::vector<std::string> statements;
	cSymbolTable symbolTable;
};

