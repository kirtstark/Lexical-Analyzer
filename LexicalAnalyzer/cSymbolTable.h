/************************************************************************
* Class: cAnalyzer
*
* Constructors:
*	static cSymbolTable* getInstance()
*		This method is used to create a singleton instance of the object
*		using a private constructor
*	cSymbolTable()
*
* Mutators:
*	bool findSymbolAndUpdate(std::string inName, std::string inValue = "")
*		This method searches for the name in the list, updating the information if
*		the object is found, and adding it if it is not found
*	bool addSymbol(std::string inName, std::string inValue = 0)
*		This method searches the list for the name, and adds the new object if 
*		it does not find it, otherwise it returns false
*	bool removeSymbol(std::string inName)
*		This method searches the list for the name and removes it if found
*	bool removeSymbol(std::list<cSymbol>::iterator it)
*		This method checks that the iterator is within the list and removes the
*		object if it is, and returns false otherwise
*
* Methods:
*   cSymbol* findSymbol(std::string inName)
*		This method searches the list for the name and returns a pointer
*		to the object if it is found, and otherwise returns NULL
*	void display()
*		This method goes through the list and calls the object display method for each
*************************************************************************/


#pragma once

#include <iostream>
#include <string>
#include <list>
#include "cSymbol.h"

class cSymbolTable
{
public:	

	// default constructor
	cSymbolTable();

	// default deconstructor
	~cSymbolTable();

	// finds the cSymbol object in the list and returns a pointer to it
	cSymbol* findSymbol(std::string inName);

	// finds the cSymbol object in the list and returns the value
	std::string findSymbolValue(std::string inName);

	// Searches for and updates the cSymbol object in the list
	bool findSymbolAndUpdate(std::string inName, std::string inValue = "", std::string inType = "");

	// Searches for and updates the cSymbol object in the list or adds it to the list if it is not found
	bool addSymbol(std::string inName, std::string inValue = "", std::string inType = "");
	bool addSymbol(std::string inName, int use, std::string inValue, std::string inType, std::string fScope, int inScope);

	// Finds the cSymbol object in the list and removes it if it is found
	bool removeSymbol(std::string inName);

	// compares a cSymbol object with those in the table and returns true if it is found or false if it is not
	bool compareSymbol(cSymbol inSymbol);

	// checks to see if given variable is in the table within current scope
	bool varIsInTable(std::string inName, std::string inFScope, int inInnerScope);

	// check to see if a given function name is in the table
	bool funIsInTable(std::string inName);

	// Removes the cSymbol object from the list based on its iterator position
	bool removeSymbol(std::list<cSymbol>::iterator it);

	// Cycles through the list and calls the display() method for each cSymbol object
	void display();

	// adds a function to the symbol table
	bool addFunction(std::string inName, std::string inType);

	std::list<cSymbol> table;

};

