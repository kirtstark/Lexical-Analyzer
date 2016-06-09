/************************************************************************
* Class: cSymbol
*
* Constructors:
*	cSymbol(std::string inVarName, std::string inValue = "")
*
* Mutators:
*	NA
*
* Methods:
*   void display()
*		This method prints the object to the screen, varName and value
*************************************************************************/

#pragma once
#include <iostream>
#include <string>

class cSymbol
{
public:
	enum Type{ an_int, a_long, a_double, a_float, a_char, a_string, string_literal, a_keyword, identifier, unk };
	enum Use{ variable_name, constant_name, type_name, function_name };

	// class constructor
	cSymbol(std::string inVarName, std::string inValue = "", std::string inType = "unk");

	// class constructor
	cSymbol(std::string inVarName, std::string inValue, std::string inType, int use);

	// class constructor
	cSymbol(int func, std::string inFunctionName, std::string inType);

	// default deconstdructor
	~cSymbol();

	// prints the class variables to the screen
	void display();

	// translates an enum Type to a string value
	static std::string TypeToString(Type inType);

	// translates a string value for a type to a Type enum
	static Type stringToType(std::string inStringType);

	

	std::string varName;
	std::string value;
	std::string functionScope;
	int innerScope;	
	
	Type type;
	Use use;

};

