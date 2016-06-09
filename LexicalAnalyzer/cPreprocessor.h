#pragma once
#include "cToken.h"
#include "cSymbol.h"
#include "cSymbolTable.h"
#include <vector>
#include "cFileData.h"

/************************************************************************
* Class: cPreprocessor
*
* Constructors:
*	cPreprocessor();
* Mutators:
*	NA
* Methods:
*	bool process(std::vector<cToken> &parsedFile);
*		Takes a  vector of cTokens and removes all preprocessor statements,
*		adds define statement equalities to the cSymbolTable list
*		and removes undef statements from the list, substituting the 
*		equalities into the program where they appear.
*************************************************************************/
class cPreprocessor
{
public:
	// default constructor
	cPreprocessor();

	// default deconstructor
	~cPreprocessor();

	// Processes through the preprocessor statements
	bool process(cFileData &dataFile);

private:
	int state1 = 0;
	std::string preprocessorType = "preprocessor";
	std::string preprocessorName = "#";
	std::string identifier = "identifier";
	std::string define = "define";
	std::string undefine = "undef";
	std::string CrgRtrnType = "Crg Rtrn";
	std::string CrgRtrnName = "RETURN";
	std::string spacesType = "spaces";
	std::string spacesName = "SPACES";
	int counter;
};

