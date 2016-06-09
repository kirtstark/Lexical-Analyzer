#include "stdafx.h"
#include "cPreprocessor.h"


/**************************************************************
*	  Purpose:  Class constructor
*
*         Entry:  NA
*
*          Exit:  NA
****************************************************************/
cPreprocessor::cPreprocessor()
{
	counter = 0;
}

/**************************************************************
*	  Purpose:  class deconstructor
*
*         Entry:  NA
*
*          Exit:  NA
****************************************************************/
cPreprocessor::~cPreprocessor()
{
}

/**************************************************************
*	  Purpose:  Saves equalities specified by define statements 
*					in the symbol table (cSymbolTable.cpp)
*					and removes equalities specified by undef
*					statements if they are found in the table
*					and currently removes any other preprocessor
*					statements without further action.
*					Equalities in the cSymbolTable class table
*					are then substituted into the program where
*					they appear.
*					Currently this method can only handle
*					define statements with two tokens.
*
*         Entry:  Method takes a vector of cToken objects
*					that contains the parsed program.
*
*          Exit:  NA
****************************************************************/
bool cPreprocessor::process(cFileData &dataFile)
{
	std::vector<cToken>::iterator it = dataFile.parsedFile.begin();
	cSymbolTable* table = new cSymbolTable();
	std::string csName = "";
	std::string csValue = "";
	std::string csType = "";

	while (it != dataFile.parsedFile.end())
	{
		if (it->getType() == identifier && (state1 > 10 || state1 == 0))
		{
			cSymbol* cs = table->findSymbol(it->getName());
			if (cs)
			{
				it->setName(cs->value);
				it->setType(cSymbol::TypeToString(cs->type));
			}
		}

		if (it->getType() == CrgRtrnType && it->getName() == CrgRtrnName && state1 != 0)
		{
			it = dataFile.parsedFile.erase(it);
			state1 = 0;

			if (csName != "")
			{
				table->addSymbol(csName, csValue, csType);
			}
				

			csName = csValue = csType = "";
		}
		else if (it->getType() == preprocessorType && it->getName() == preprocessorName)
		{
			it = dataFile.parsedFile.erase(it);
			state1 = 1;
		}
		else if (state1 == 1 && it->getType() == identifier && it->getName() == define)
		{
			it = dataFile.parsedFile.erase(it);
			state1 = 10;
		}
		else if (state1 == 1 && it->getType() == identifier && it->getName() == undefine)
		{
			it = dataFile.parsedFile.erase(it);

			while (it->getType() != identifier && it->getType() != CrgRtrnType && it->getName() != CrgRtrnName)
			{
				it = dataFile.parsedFile.erase(it);
			}
			
			if (it->getType() == identifier)
			{
				table->removeSymbol(it->getName());
				it = dataFile.parsedFile.erase(it);
			}

			while (it->getType() != CrgRtrnType && it->getName() != CrgRtrnName)
			{
				it = dataFile.parsedFile.erase(it);
				it++;
			}

			state1 = 0;
		}
		else if (state1 == 10 && it->getType() == identifier)
		{
			csName = it->getName();
			it = dataFile.parsedFile.erase(it);
			state1 = 11;
		}
		else if (state1 == 11 && it->getType() != spacesType && it->getName() != spacesName)
		{
			csValue = it->getName();
			csType = it->getType();
			it = dataFile.parsedFile.erase(it);
			state1 = 12;
		}
		else if (state1 != 0)  // used to remove any other preprocessor statements or spaces - modify as other statements are processed
		{
			it = dataFile.parsedFile.erase(it);
		}
		else
			it++;
	}

	// For debug purposes only
	
	std::cout << std::endl << std::endl;
	std::cout << "This is the table for file " << ++counter << std::endl << std::endl;
	table->display();
	std::cout << std::endl << std::endl;
	
	

	delete table;
	return true;
}
