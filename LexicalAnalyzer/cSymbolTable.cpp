#include "stdafx.h"
#include "cSymbolTable.h"



/**************************************************************
*	  Purpose:  Default constructor
*
*         Entry:  NA
*
*          Exit:  NA
****************************************************************/
cSymbolTable::cSymbolTable()
{
//	table = new std::list<cSymbol>();
}

/**************************************************************
*	  Purpose:  Default deconstrutor, resets the instanceFlag
*				to false and calls the deconstructor for the table
*
*         Entry:  NA
*
*          Exit:  NA
****************************************************************/
cSymbolTable::~cSymbolTable()
{
//	delete table;
}


/**************************************************************
*	  Purpose:  Locates the cSymbol object in the list and returns
*				a pointer to its location
*
*         Entry:  Requires string value.
*
*          Exit:  NA
****************************************************************/
cSymbol* cSymbolTable::findSymbol(std::string inName)
{
	std::list<cSymbol>::iterator it = table.begin();
	while (it != table.end())
	{
		if (it->varName == inName)
		{
			return &*it;
		}
		it++;
	}

	return NULL;
}

/**************************************************************
*	  Purpose:  compares a symbol to the list and returns true if it finds a scope and name match
*
*         Entry:  Requires cSymbol value.
*
*          Exit:  NA
****************************************************************/
bool cSymbolTable::compareSymbol(cSymbol inSymbol)
{
	std::list<cSymbol>::iterator it = table.begin();
	while (it != table.end())
	{
		if (it->varName == inSymbol.varName && it->functionScope == inSymbol.functionScope && it->innerScope == inSymbol.innerScope)
			return true;
		it++;
	}

	return false;
}

/**************************************************************
*	  Purpose:  Locates the cSymbol object in the list and returns
*				the value associated with it
*
*         Entry:  Requires string value.
*
*          Exit:  NA
****************************************************************/
std::string cSymbolTable::findSymbolValue(std::string inName)
{
	std::list<cSymbol>::iterator it = table.begin();
	while (it != table.end())
	{
		if (it->varName == inName)
		{
			return it->value;
		}
		it++;
	}

	return "";
}

/**************************************************************
*	  Purpose:  Locates the cSymbol object in the list and updates
*				with new values
*
*         Entry:  Requires string value for object name and a string value for the object value.
*
*          Exit:  NA
****************************************************************/
bool cSymbolTable::findSymbolAndUpdate(std::string inName, std::string inValue, std::string inType)
{
	std::list<cSymbol>::iterator it = table.begin();
	while (it != table.end())
	{
		if (it->varName == inName)
		{
			it->value = inValue;
			it->type = it->stringToType(inType);
			return true;
		}
		it++;
	}

	return false;
}

/**************************************************************
*	  Purpose:  Searches for the cSymbol object by name in the list,
*				updates the values if it is found or adds the new object
*				if it is not found
*
*         Entry:  Requires string value for object name and a string value for the object value.
*
*          Exit:  NA
****************************************************************/
bool cSymbolTable::addSymbol(std::string inName, std::string inValue, std::string inType)
{
	if (!findSymbolAndUpdate(inName, inValue, inType))
	{
		table.push_back(cSymbol(inName, inValue, inType));
		return false;
	}
		
	return true;
}

bool cSymbolTable::addSymbol(std::string inName, int use, std::string inValue, std::string inType, std::string fScope, int inScope)
{
	cSymbol temp(inName, inValue, inType, use);
	temp.functionScope = fScope;
	temp.innerScope = inScope;
	if (compareSymbol(temp))
	{
		std::cout << "ERROR - variable name " << inName << " re-defined" << std::endl;
		return false;
	}

	table.push_back(temp);
	return true;
}

/**************************************************************
*	  Purpose:  Check to see if variable is within current scope
*
*         Entry:  Requires a string value for name and function scope
*					and an int value for inner scope
*
*          Exit:  NA
****************************************************************/
bool cSymbolTable::varIsInTable(std::string inName, std::string inFScope, int inInnerScope)
{
	std::list<cSymbol>::iterator it = table.begin();
	while (it != table.end())
	{
		if (it->varName == inName && it->functionScope == inFScope && it->innerScope <= inInnerScope)
			return true;
		it++;
	}

	return false;
}

/**************************************************************
*	  Purpose:  Check to see if function is in table
*
*         Entry:  Requires a string value for name and function scope
*					and an int value for inner scope
*
*          Exit:  NA
****************************************************************/
bool cSymbolTable::funIsInTable(std::string inName)
{
	std::list<cSymbol>::iterator it = table.begin();
	while (it != table.end())
	{
		if (it->varName == inName && it->use == cSymbol::function_name)
			return true;
		it++;
	}

	return false;
}

/**************************************************************
*	  Purpose:  Searches for the cSymbol object by name in the list,
*				and removes it if it is found, or returns false
*				if it is not found
*
*         Entry:  Requires string value for object name 
*
*          Exit:  NA
****************************************************************/
bool cSymbolTable::removeSymbol(std::string inName)
{
	std::list<cSymbol>::iterator it = table.begin();
	while (it != table.end())
	{
		if (it->varName == inName)
		{
			table.erase(it);
			return true;
		}
		it++;
	}
	
	return false;
}

/**************************************************************
*	  Purpose:  Searches for the cSymbol object using its iterator position in the list,
*				and removes it if it is found, or returns false
*				if it is not found
*
*         Entry:  Requires a list<cSymbol>::iterator
*
*          Exit:  NA
****************************************************************/
bool cSymbolTable::removeSymbol(std::list<cSymbol>::iterator it)
{
	if ((std::distance(table.begin(), it)) > 0 && (std::distance(table.begin(), it)) <= table.size())
		table.erase(it);
	else
		return false;

	return true;
}

/**************************************************************
*	  Purpose:  Cycles through all of the cSymbol objects in the list
*				and calls the display function for each one
*
*         Entry:  NA
*
*          Exit:  NA
****************************************************************/
void cSymbolTable::display()
{
	std::list<cSymbol>::iterator it = table.begin();
	while (it != table.end())
	{
		it->display();
		++it;
	}
	std::cout << std::endl << std::endl;
}

/**************************************************************
*	  Purpose:  adds a function name to the symbol table
*
*         Entry:  NA
*
*          Exit:  NA
****************************************************************/
bool cSymbolTable::addFunction(std::string inName, std::string inType)
{
	cSymbol temp(1, inName, inType);

	if (funIsInTable(inName))
	{
		std::cout << "ERROR - function name " << inName << " re-defined" << std::endl;
		return false;
	}

	table.push_back(temp);
	return true;
}
