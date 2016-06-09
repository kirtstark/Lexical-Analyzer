#include "stdafx.h"
#include "cSymbol.h"



/**************************************************************
*	  Purpose:  Class constructor
*
*         Entry:  Requires string value for object name and a string value
*					for the object value
*
*          Exit:  NA
****************************************************************/
cSymbol::cSymbol(std::string inVarName, std::string inValue, std::string inType)
{
	varName = inVarName;
	value = inValue;
	type = stringToType(inType);
}

/**************************************************************
*	  Purpose:  Class constructor
*
*         Entry:  Requires string value for object name and a string value
*					for the object value and a Use variable
*
*          Exit:  NA
****************************************************************/
cSymbol::cSymbol(std::string inVarName, std::string inValue, std::string inType, int inUse)
{
	varName = inVarName;
	value = inValue;
	type = stringToType(inType);
	switch (inUse)
	{
	case 0: use = variable_name;
		break;
	case 1: use = constant_name;
		break;
	case 2: use = type_name;
		break;
	case 3: use = function_name;
		break;
	default:
		use = variable_name;
	}
}

cSymbol::cSymbol(int func, std::string inFunctionName, std::string inType)
{
	varName = inFunctionName;
	use = function_name;
	type = stringToType(inType);
}

/**************************************************************
*	  Purpose:  class deconstructor
*
*         Entry:  NA
*
*          Exit:  NA
****************************************************************/
cSymbol::~cSymbol()
{
}

/**************************************************************
*	  Purpose:  Prints the object values to the screen
*
*         Entry:  NA
*
*          Exit:  NA
****************************************************************/
void cSymbol::display()
{
	std::cout << "Name:  " << varName << "        Value:  " << value << "        Type:  " << type << std::endl;
}

/**************************************************************
*	  Purpose:  Prrovides a way to translate a Type enum type
*				to a string for the cToken class to use
*
*         Entry:  NA
*
*          Exit:  NA
****************************************************************/
std::string cSymbol::TypeToString(Type inType)
{
	switch (inType)
	{
	case an_int: return "integer";
		break;
	case a_long: return "long";
		break;
	case identifier: return "identifier";
		break;
	case a_double: return "double";
		break;
	case a_float: return "float";
		break;
	case a_char: return "char";
		break;
	case a_string: return "string";
		break;
	case string_literal: return "str literal";
		break;
	case a_keyword: return "keyword";
		break;
	default: return "";
	}
}

/**************************************************************
*	  Purpose:  Translates the cToken string type into an enum
*				type for this class to use
*
*         Entry:  NA
*
*          Exit:  NA
****************************************************************/
cSymbol::Type cSymbol::stringToType(std::string inStringType)
{
	if (inStringType == "int" || inStringType == "integer")
		return an_int;
	else if (inStringType == "long")
		return a_long;
	else if (inStringType == "double")
		return a_double;
	else if (inStringType == "float")
		return a_float;
	else if (inStringType == "char")
		return a_char;
	else if (inStringType == "string")
		return a_string;
	else if (inStringType == "str literal")
		return string_literal;
	else if (inStringType == "keyword")
		return a_keyword;
	else if (inStringType == "identifier")
		return identifier;
	else
		return unk;
}

