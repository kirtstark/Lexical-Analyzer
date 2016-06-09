#include "stdafx.h"
#include "cToken.h"
#include <iostream>
#include <iomanip>

/**************************************************************
*	  Purpose:  Defualt constructor
*
*         Entry:  NA
*
*          Exit:  NA
****************************************************************/
cToken::cToken()
{
}

/**************************************************************
*	  Purpose:  Constructs cToken object and sets name and type values.
*
*         Entry:  Requires name and type string values.
*
*          Exit:  Returns initialized cToken object.
****************************************************************/
cToken::cToken(std::string _name, std::string _type)
{
	setName(_name);
	setType(_type);
}

/**************************************************************
*	  Purpose:  Default deconstructor.
*
*         Entry:  NA
*
*          Exit:  NA
****************************************************************/
cToken::~cToken()
{
}

/**************************************************************
*	  Purpose:  Initializes the object name value.
*
*         Entry:  Requires string value.
*
*          Exit:  NA
****************************************************************/
void cToken::setName(std::string _name)
{
	name = _name;
}

/**************************************************************
*	  Purpose:  Returns the object's name value.
*
*         Entry:  NA
*
*          Exit:  NA
****************************************************************/
std::string cToken::getName()
{
	return name;
}

/**************************************************************
*	  Purpose:  Initializes the object type value.
*
*         Entry:  Requires string value.
*
*          Exit:  NA
****************************************************************/
void cToken::setType(std::string _type)
{
	// NOTE: future revisions should check the _type is a valid "type" value prior to setting.
	type = _type;
}

/**************************************************************
*	  Purpose:  Returns the object's type value.
*
*         Entry:  NA
*
*          Exit:  NA
****************************************************************/
std::string cToken::getType()
{
	return type;
}

/**************************************************************
*	  Purpose:  Displays an object's name and type in table 
*				format to the screen
*
*         Entry:  NA
*
*          Exit:  NA
****************************************************************/
void cToken::display()
{
	std::cout << std::setw(12) << std::left << type << std::setw(8) << " " << std::left << name << std::endl;
}

/**************************************************************
*	  Purpose:  Displays a table header that is compatible
*				with the display method.
*
*         Entry:  NA
*
*          Exit:  NA
****************************************************************/
void cToken::tableTitles()
{
	std::cout << std::endl << std::endl << "Type" << std::setw(18) << " " << "Name" << std::endl;
	std::cout << std::setfill('-') << std::setw(40) << "-" << std::endl;
	std::cout << std::setfill(' ');
}