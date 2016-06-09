#include <conio.h>


#include "stdafx.h"
#include "cCompileCheck.h"
#include <iostream>
#include <string>

/**************************************************************
*	  Purpose:  Class constructor
*
*         Entry:  NA
*
*          Exit:  NA
****************************************************************/
cCompileCheck::cCompileCheck()
{
	data_type.insert("bool");
	data_type.insert("char");
	data_type.insert("double");
	data_type.insert("float");
	data_type.insert("int");
	data_type.insert("integer");
	data_type.insert("long");
	data_type.insert("short");
	data_type.insert("void");
	data_type.insert("string");

	errorMessageTemp = errorMessage;
}

/**************************************************************
*	  Purpose:  class deconstructor
*
*         Entry:  NA
*
*          Exit:  NA
****************************************************************/
cCompileCheck::~cCompileCheck()
{
}

/**************************************************************
*	  Purpose:  Entry point for class, takes the cToken vector in 
*				and updates it in the class.
*
*         Entry:  Requires an input of a vector of cToken objects.
*			assumes cTokens are parsed c program.
*
*          Exit:  Returns a bool value to indicate validity of the 
*				  program. Prints out any error message.
****************************************************************/
bool cCompileCheck::programCheck(cFileData& datafile)
{
	pointDataFile = &datafile;
	errorMessage = "unknown error";
	errorMessageTemp = errorMessage;
	lineNum = 1;
	parsedFile = datafile.parsedFile;
	it = parsedFile.begin();
	if (program())
		return true;
	else
	{
		std::cout << "ERROR at line " << lineNum << ":    " << errorMessage << std::endl;
		if (it == parsedFile.end())
			std::cout << "End of file reached" << std::endl;
		else
			std::cout << "Last token seen:  " << it->getName() << "  " << it->getType() << std::endl;
		return false;
	}
}

/**************************************************************
*	  Purpose:  beginning point, calls all other programs and processes
*			through entire vector of tokens to determine any
*			syntax errors, returning first error found or that file is good.
*
*         Entry:  Requires a current vector of cToken objects are
*			already loaded into the class.
*
*          Exit:  Returns a bool value to indicate validity of the
*				  program. 
****************************************************************/
bool cCompileCheck::program()
{
	bool result = false;

	if (clearSpaces(true) == -1)
	{
		makeBooBoo("No program located in file");
		return false;
	}

	while (it != parsedFile.end())
	{
		// could we save global variables here, as well?

		if (function_header())
		{			
			pointDataFile->statements.push_back("Function Declaration");
			if (function_body())
			{
				result = true;
			}
			else
			{
				makeBooBoo("Invalid function body");
				return false;
			}				
		}
		else
		{
			makeBooBoo("Invalid function header");
			return false;
		}			
	}
	return result;
}

/**************************************************************
*	  Purpose:  tests any and all function headers for syntax errors.
*
*         Entry:  Requires a current vector of cToken objects are
*			already loaded into the class.
*
*          Exit:  Returns a bool value to indicate the results of 
*				the method's validity tests.
****************************************************************/
bool cCompileCheck::function_header()
{
	std::string functionType = "void";
	std::string functionName = "";
	std::string functionValue = "function";

	if (clearSpaces(true) == -1)
		return false;

	if (it->getType() == "keyword")
	{
		std::hash_set<std::string>::iterator hashIt = data_type.find(it->getName());
		if (hashIt != data_type.end())
		{
			functionType = it->getName();
			if (!advanceIt())
				return false;
		}			
		else
		{
			makeBooBoo("function header has wrong data return type");
			return false;
		}		
	}

	if (clearSpaces(true) == -1)
		return false;

	if (it->getType() == identifier)
	{
		functionName = it->getName();
		functionScope = functionName;

		if (!advanceIt())
			return false;

		if (it->getName() == "(")
		{
			if (!advanceIt())
				return false;

			if (parameter_list())
			{
				if (it->getName() == ")")
				{
					if (!advanceIt())
						return false;

					if (!pointDataFile->symbolTable.addFunction(functionName, functionType))
					{
						makeBooBoo("Function name redefinition");
						return false;
					}
					return true;
				}
				else
				{
					makeBooBoo("parameter list is missing end parenthesis \')\'");
					return false;
				}
			}
			else
				return false;
		}
		else
		{
			makeBooBoo("function missing parameter list \'(\'");
			return false;
		}
	}
	else
	{
		makeBooBoo("bad function name");
		return false;
	}
}

/**************************************************************
*	  Purpose:  tests the function bodies for syntax errors.
*
*         Entry:  Requires a current vector of cToken objects are
*			already loaded into the class.
*
*          Exit:  Returns a bool value to indicate the results of
*				the method's validity tests.
****************************************************************/
bool cCompileCheck::function_body()
{
	if (clearSpaces(true) == -1)
		return false;

	if (it->getName() == "{")
	{
		innerScope++;
		if (!advanceIt())
			return false;

		while (it != parsedFile.end() && it->getName() != "}")
		{
			if (statement_list())
			{
				if (!advanceIt())
					return false;
			}
			else
			{
				makeBooBoo("unrecognized or missing statement");
				return false;
			}
		}

		if (it == parsedFile.end())
		{
			makeBooBoo("unexpected end of file");
			return false;
		}

		if (it->getName() == "}")
		{
			it++;
			clearSpaces(false);
			return true;
		}
		else
		{
			makeBooBoo("function body missing \'}\'");
			return false;
		}
	}
	else
	{
		makeBooBoo("function body missing \'{\'");
		return false;
	}
	
	innerScope--;
	return true;
}

/**************************************************************
*	  Purpose:	loops through a list of statements and tests for syntax errors.
*
*         Entry:  Requires a current vector of cToken objects are
*			already loaded into the class.
*
*          Exit:  Returns a bool value to indicate the results of
*				the method's validity tests.
****************************************************************/
bool cCompileCheck::statement_list()
{
	thisStatementType = "";
	if (it->getName() == "{")
	{
		innerScope++;
		if (!advanceIt())
			return false;

		while (it != parsedFile.end() && it->getName() != "}")
		{
			if (statement_list())
			{
				if (!advanceIt())
					return false;
			}
			else
			{
				makeBooBoo("error in statement list");
				return false;
			}
		}
		innerScope--;
		return true;
	}
	else if (statement())
	{
		pointDataFile->statements.push_back(thisStatementType);
		return true;
	}
	else
	{
		makeBooBoo("unrecognized statement");
		return false;
	}	
}

/**************************************************************
*	  Purpose:	tests an individual statement for syntax errors.
*
*         Entry:  Requires a current vector of cToken objects are
*			already loaded into the class.
*
*          Exit:  Returns a bool value to indicate the results of
*				the method's validity tests.
****************************************************************/
bool cCompileCheck::statement()
{
	//std::cout << "line- " << lineNum << "   var:: " << it->getName() << "  type:::  " << it->getType() << std::endl;/////////////////////////////////////////////
	errorMessageTemp = errorMessage;
	if (it->getType() == identifier)
	{
		varList.clear();
		if (f_call())
		{
			thisStatementType = "Function call";
			return true;
		}
		else if (var_list())
		{
			for each(std::string var in varList)
			{
				if (!pointDataFile->symbolTable.varIsInTable(var, functionScope, innerScope))
				{
					makeBooBoo("undefined variable");
					return false;
				}
			}

			if (it->getName() == "=")
			{
				if (!advanceIt())
					return false;
				if (exp())
				{
					if (it->getName() == ";")
					{
						thisStatementType = "Assignment Statement";
						return true;
					}
					else
					{
						makeBooBoo("expected a semicolon but none found");
						return false;
					}
				}
				else
				{
					makeBooBoo("invalid expression");
					return false;
				}
			}
			else
			{
				makeBooBoo("missing an operator such as \'=\' ");
				return false;
			}
		}
		else
		{
			makeBooBoo("error in variable or variable list");
			return false;
		}
	}
	else if (it->getName() == ";")
	{
		thisStatementType = "Empty Statement";
		return true;
	}
	else if (it->getType() == keyword && isDataType())
	{
		if (it->getName() == "void")
		{
			makeBooBoo("incorrect statement data declaration");
			return false;
		}

		varType = it->getName();

		if (!advanceIt())
			return false;

		if (it->getType() == identifier)
		{
			varList.clear();
			if (var_list())
			{
				if (it->getName() == "=")
				{
					if (!advanceIt())
						return false;
					if (exp())
					{					
						if (it->getName() == ";")
						{
							for each (std::string  var in varList)
							{
								if (!pointDataFile->symbolTable.addSymbol(var, 0, "", varType, functionScope, innerScope))
								{
									makeBooBoo("variable redefinition");
									return false;
								}
							}
							thisStatementType = "Data declaration and Assignement";
							return true;
						}
						else
						{
							makeBooBoo("semicolon expected");
							return false;
						}
					}
					else
					{
						makeBooBoo("expression expected");
						return false;
					}
				}

				else if (it->getName() == ";")
				{
					for each (std::string  var in varList)
					{
						if (!pointDataFile->symbolTable.addSymbol(var, 0, "", varType, functionScope, innerScope))
						{
							makeBooBoo("variable redefinition");
							return false;
						}
					}
					thisStatementType = "Data Declaration";
					return true;
				}
				else
				{
					makeBooBoo("end of statement expected");
					return false;
				}
			}
			else
			{
				makeBooBoo("identifier expected");
				return false;
			}
		}
		else
		{
			makeBooBoo("identifier expected");
			return false;
		}
	}
	else if (it->getType() == anOperator)
	{
		if (it->getName() == "++" || it->getName() == "+=" || it->getName() == "--" || it->getName() == "-=" || it->getName() == "*=" || it->getName() == "/=")
		{
			if (!advanceIt())
				return false;
			if (it->getType() == identifier)
			{
				if (!advanceIt())
					return false;
				if (it->getName() == ";")
				{
					thisStatementType = "Variable adjustment";
					return true;
				}
				else
				{
					makeBooBoo("semicolon expected");
					return false;
				}
			}
			else
			{
				makeBooBoo("identifier expected");
				return false;
			}
		}
		else
		{
			makeBooBoo("invalid operator at first of statement");
			return false;
		}
	}
	else if (it->getType() == keyword)
	{
		if (it->getName() == "printf")
		{
			if (!advanceIt())
				return false;
			if (it->getName() == "(")
			{
				if (!advanceIt())
					return false;
				if (string_list())
				{
					if (it->getName() == ")")
					{
						if (!advanceIt())
							return false;
						if (it->getName() == ";")
						{
							thisStatementType = "Output Statment";
							return true;
						}
						else
						{
							makeBooBoo("semicolon expected");
							return false;
						}
					}
					else
					{
						makeBooBoo("closing parenthesis expected");
						return false;
					}
				}
				else
				{
					makeBooBoo("unrecognized string in statement");
					return false;
				}
			}
			else
			{
				makeBooBoo("unrecognized statement");
				return false;
			}
		}
		else if (it->getName() == "scanf")
		{
			if (!advanceIt())
				return false;
			if (it->getName() == "(")
			{
				if (!advanceIt())
					return false;
				if (var_list())
				{
					if (it->getName() == ")")
					{
						if (!advanceIt())
							return false;
						if (it->getName() == ";")
						{
							thisStatementType = "Input Statement";
							return true;
						}
						else
						{
							makeBooBoo("semicolon expected");
							return false;
						}
					}
					else
					{
						makeBooBoo("closing parenthesis expected");
						return false;
					}
				}
				else
				{
					makeBooBoo("unrecognized string in statement");
					return false;
				}
			}
			else
			{
				makeBooBoo("unrecognized statement");
				return false;
			}
		}
		else if (it->getName() == "if")
		{
			pointDataFile->statements.push_back("if statement");
			if (comp_statements())
			{
				std::vector<cToken>::iterator temp_it = it;

				if (!advanceIt())
					return false;
				
				if (it->getName() == "else")
				{
					bool done_a = false;
					if (!advanceIt())
						return false;

					if (it->getName() == "if")
					{
						pointDataFile->statements.push_back("else-if statement");
						bool done_b = false;
						while (!done_b)
						{
							if (comp_statements())
							{
								temp_it = it;

								if (!advanceIt())
									return false;
								
								if (it->getName() == "else")
								{
									temp_it = it;

									if (!advanceIt())
										return false;

									if (it->getName() == "if")
									{
										if (!advanceIt())
											return false;
									}
									else
									{
										pointDataFile->statements.push_back("else statement");
										it = temp_it;
										done_b = true;
									}
								}
								else
								{
									it = temp_it;
									done_a = true;
									done_a = true;
								}
							}
							else
							{
								makeBooBoo("invalid if statement");
								return false;
							}

						}
					}

					if (!done_a)
					{
						if (it->getName() == "{")
						{
							innerScope++;
							if (statement_list())
							{
								if (it->getName() == "}")
								{
									innerScope--;
									return true;
								}
								else
								{
									makeBooBoo("closing brackets expected");
									return false;
								}
							}
							else
							{
								makeBooBoo("statements expected");
								return false;
							}
						}
					}
					pointDataFile->statements.push_back("else Statement");
					return true;
				}
				else
				{
					it = temp_it;
					return true;
				}
			}
			else
			{
				makeBooBoo("invalid if statement");
				return false;
			}

		}
		else if (it->getName() == "while")
		{
			pointDataFile->statements.push_back("while Statement");
			if (comp_statements())
				return true;
			else
			{
				makeBooBoo("invalid while statement");
				return false;
			}
		}
		else if (it->getName() == "return")
		{
			thisStatementType = "return Statement";
			if (!advanceIt())
				return false;
			if (expression())
			{
				if (it->getName() == ";")
					return true;
				else
				{
					makeBooBoo("expected semicolon");
					return false;
				}
			}
			else if (value())
			{
				if (it->getName() == ";")
					return true;
				else
				{
					makeBooBoo("expected semicolon");
					return false;
				}
			}
			else
			{
				makeBooBoo("unrecognized return value");
				return false;
			}
		}
		else
		{
			makeBooBoo("invalid keyword use in statement");
			return false;
		}
	}
	
	else
	{
		makeBooBoo("unrecognized statement");
		return false;
	}
}

/**************************************************************
*	  Purpose:	tests for a comparrision and for syntax errors.
*
*         Entry:  Requires a current vector of cToken objects are
*			already loaded into the class.
*
*          Exit:  Returns a bool value to indicate the results of
*				the method's validity tests.
****************************************************************/
bool cCompileCheck::comp_stat()
{
	if (it->getName() == "!")
	{
		if (!advanceIt())
			return false;
	}
	if (value())
	{
		if (!advanceIt())
			return false;
		if (it->getName() == "==" || it->getName() == "<>" || it->getName() == ">" || it->getName() == "<" || it->getName() == ">=" || it->getName() == "<=" || it->getName() == "!=")
		{
			if (!advanceIt())
				return false;
			if (value())
			{
				if (!advanceIt())
					return false;
				return true;
			}
			else
			{
				makeBooBoo("no comparitor");
				return false;
			}
		}
		else
			return true;
	}
	else
	{
		makeBooBoo("no comparisson values");
		return false;
	}
}

/**************************************************************
*	  Purpose:	tests for syntax error throughout an entire comparrisonn statement.
*
*         Entry:  Requires a current vector of cToken objects are
*			already loaded into the class.
*
*          Exit:  Returns a bool value to indicate the results of
*				the method's validity tests.
****************************************************************/
bool cCompileCheck::comp_statements()
{
	if (!advanceIt())
		return false;
	if (it->getName() == "(")
	{
		if (!advanceIt())
			return false;
		if (comp_stat())
		{
			if (!advanceIt())
				return false;
			if (it->getName() == ")")
			{
				if (!advanceIt())
					return false;
				if (it->getName() == "{")
				{
					innerScope++;
					if (statement_list())
					{
						if (it->getName() == "}")
						{
							innerScope--;
							return true;
						}
						else
						{
							makeBooBoo("closing brackets expected");
							return false;
						}
					}
					else
					{
						makeBooBoo("statements expected");
						return false;
					}
				}
				else
				{
					makeBooBoo("opening brackets expected");
					return false;
				}
			}
			else
			{
				makeBooBoo("closing parenthesis expected");
				return false;
			}
		}
		else
		{
			makeBooBoo("comparrison expected");
			return false;
		}
	}
	else
	{
		makeBooBoo("opening parenthesis expected");
		return false;
	}
}

/**************************************************************
*	  Purpose:	tests to determine if it is a list of strings.
*
*         Entry:  Requires a current vector of cToken objects are
*			already loaded into the class.
*
*          Exit:  Returns a bool value to indicate the results of
*				the method's validity tests.
****************************************************************/
bool cCompileCheck::string_list()
{
	bool done = false;
	while (it != parsedFile.end() && !done && (it->getType() == "str literal" || it->getType() == identifier))
	{
		if (!advanceIt())
			return false;

		if (it->getName() == ",")
		{
			if (!advanceIt())
				return false;
		}
		else
			done = true;
	}
	return true;
}

/**************************************************************
*	  Purpose:	test if it has a function call and tests  for syntax errors
*
*         Entry:  Requires a current vector of cToken objects are
*			already loaded into the class.
*
*          Exit:  Returns a bool value to indicate the results of
*				the method's validity tests.
****************************************************************/
bool cCompileCheck::f_call()
{
	if (it->getType() == identifier)
	{
		std::string tempFuncName = it->getName();
		std::vector<cToken>::iterator temp_it = it;

		if (!advanceIt())
			return false;

		if (it->getName() == "(")
		{
			if (pointDataFile->symbolTable.funIsInTable(tempFuncName))
			{
				if (value_list())
				{
					if (it->getName() == ")")
					{
						if (!advanceIt())
							return false;
						return true;
					}
					else
					{
						makeBooBoo("expected a parenthesis");
						return false;
					}
				}
				else
				{
					makeBooBoo("missing or invalid parameter list");
					return false;
				}
			}
			else
			{
				makeBooBoo("unrecognized function name");
				return false;
			}
		}
		else
		{
			it = temp_it;
			return false;
		}
	}
	else
	{
		return false;
	}
}

/**************************************************************
*	  Purpose:	tests a list of values for syntax errors.
*
*         Entry:  Requires a current vector of cToken objects are
*			already loaded into the class.
*
*          Exit:  Returns a bool value to indicate the results of
*				the method's validity tests.
****************************************************************/
bool cCompileCheck::value_list()
{
	bool done = false;
	bool result = false;

	if (!advanceIt())
		return false;

	do{
		if (value())
		{
			result = true;
		}
		else
		{
			makeBooBoo("unrecognized value");
			return false;
		}

		if (it->getName() == ",")
		{
			if (!advanceIt())
				return false;
		}
		else
			done = true;
	} while (!done);
	return result;
}

/**************************************************************
*	  Purpose:	tests the variable lists for syntax errors.
*
*         Entry:  Requires a current vector of cToken objects are
*			already loaded into the class.
*
*          Exit:  Returns a bool value to indicate the results of
*				the method's validity tests.
****************************************************************/
bool cCompileCheck::var_list()
{
	bool done = false;
	bool result = false;

	if (clearSpaces(true) == -1)
		return false;

	do{
		if (it->getType() == identifier)
		{
			varList.push_back(it->getName());
			result = true;

			if (!advanceIt())
				return false;
		}
		else
		{
			makeBooBoo("unrecognized value");
			return false;
		}

		if (it->getName() == ",")
		{
			if (!advanceIt())
				return false;
		}
		else
			done = true;
	} while (!done);
	return result;
}

/**************************************************************
*	  Purpose:	tests the parameter lists for syntax errors
*
*         Entry:  Requires a current vector of cToken objects are
*			already loaded into the class.
*
*          Exit:  Returns a bool value to indicate the results of
*				the method's validity tests.
****************************************************************/
bool cCompileCheck::parameter_list()
{
	if (clearSpaces(true) == -1)
		return false;
	bool done = false;

	if (it->getName() != ")")
	{
		innerScope++;
		do{
			if (it->getType() == "keyword" && it->getName() != "void")
			{				
				std::hash_set<std::string>::iterator hashIt = data_type.find(it->getName());
				if (hashIt != data_type.end())
				{
					std::string varType = it->getName();
					if (!advanceIt())
						return false;

					if (it->getType() == "identifier")
					{
						std::string varName = it->getName();
						if (!pointDataFile->symbolTable.addSymbol(varName, 0, "", varType, functionScope, innerScope))
						{
							makeBooBoo("variable redefinition in parameter list");
							return false;
						}
						if (!advanceIt())
							return false;
					}
					else
					{
						makeBooBoo("unrecognized variable in parameter list");
						return false;
					}
				}
				else
				{
					makeBooBoo("function header has wrong data return type");
					return false;
				}
			}
			else
			{
				makeBooBoo("incorrect data type usage in parameter list");
				return false;
			}

			if (it->getName() == ",")
			{
				if (!advanceIt())
					return false;
			}
			else
				done = true;
		} while (!done);
	}
	innerScope--;
	return true;
}

/**************************************************************
*	  Purpose:	tests an expression, including a list of expressions, for sytax errors.
*
*         Entry:  Requires a current vector of cToken objects are
*			already loaded into the class.
*
*          Exit:  Returns a bool value to indicate the results of
*				the method's validity tests.
****************************************************************/
bool cCompileCheck::exp()
{
	if (expression_list())
		return true;
	else if (op_list())
	{
		if (it->getType() == identifier)
		{
			if (!advanceIt())
				return false;
			else
				return true;
		}
		else
		{
			makeBooBoo("missing variable");
			return false;
		}
	}
	else
	{
		makeBooBoo("expression error");
		return false;
	}
}

/**************************************************************
*	  Purpose:	tests a list of expressions for syntax errors.
*
*         Entry:  Requires a current vector of cToken objects are
*			already loaded into the class.
*
*          Exit:  Returns a bool value to indicate the results of
*				the method's validity tests.
****************************************************************/
bool cCompileCheck::expression_list()
{
	if (it->getName() == "(")
	{
		if (!advanceIt())
			return false;

		while (it != parsedFile.end() && it->getName() != ")")
		{
			if (!expression_list())
			{
				makeBooBoo("expression bad");
				return false;
			}
		}
		if (!advanceIt())
			return false;

		if (ex_list_x())
		{
			return true;
		}
		else
		{
			makeBooBoo("expression error");
			return false;
		}
	}
	else if (expression())
	{
		if (ex_list_x())
			return true;
		else
		{
			makeBooBoo("expression error");
			return false;
		}
	}
	else
	{
		makeBooBoo("expression error");
		return false;
	}
}

/**************************************************************
*	  Purpose:	an extension and helper function for expression_list to avoid left recursion.
*
*         Entry:  Requires a current vector of cToken objects are
*			already loaded into the class.
*
*          Exit:  Returns a bool value to indicate the results of
*				the method's validity tests.
****************************************************************/
bool cCompileCheck::ex_list_x()
{
	std::vector<cToken>::iterator temp_it = it;

	if (op_list())
	{
		if (expression_list())
		{
			ex_list_x();
			return true;
		}
		else
		{
			makeBooBoo("expected an expression");
			return false;
		}
	}
	else
	{
		it = temp_it;
		return true;
	}
}

/**************************************************************
*	  Purpose:	tests an individual expression for syntax errors.
*
*         Entry:  Requires a current vector of cToken objects are
*			already loaded into the class.
*
*          Exit:  Returns a bool value to indicate the results of
*				the method's validity tests.
****************************************************************/
bool cCompileCheck::expression()
{
	if (it->getName() == ")")
		return true;
	else if (value())
	{
		if (it->getName() == ")")
			return true;
		if (op_list())
		{
			if (value())
			{
				if (it->getName() == ")")
					return true;
				if (op_list())
				{
					if (expression())
						return true;
					else
					{
						makeBooBoo("missing values in expression");
						return false;
					}
				}
				else
					return true;
			}
			else if (it->getName() == "(")
			{
				if (expression_list())
					return true;
				else
				{
					makeBooBoo("unknown symbol use");
					return false;
				}
			}
			else
			{
				makeBooBoo("expected a value");
				return false;
			}
		}

		else
			return true;

	}
	else if (it->getName() == "(")
	{
		if (expression_list())
			return true;
		else
		{ 
			makeBooBoo("unknown symbol use");
			return false;
		}
	}
	else
	{
		makeBooBoo("unrecognized value in statement");
		return false;
	}
}

/**************************************************************
*	  Purpose:	tests a list of operators to see if it is all operators.
*
*         Entry:  Requires a current vector of cToken objects are
*			already loaded into the class.
*
*          Exit:  Returns a bool value to indicate the results of
*				the method's validity tests.
****************************************************************/
bool cCompileCheck::op_list()
{
	if (it->getType() == anOperator)
	{
		while (it->getType() == anOperator)
		{
			if (!advanceIt())
				return false;
		}
		return true;
	}
	else
	{
		return false;
	}
}

/**************************************************************
*	  Purpose:	tests if a token is a value.
*
*         Entry:  Requires a current vector of cToken objects are
*			already loaded into the class.
*
*          Exit:  Returns a bool value to indicate the results of
*				the method's validity tests.
****************************************************************/
bool cCompileCheck::value()
{
	std::hash_set<std::string>::iterator hashIt = data_type.find(it->getType());
	if (f_call())
		return true;	
	else if (hashIt != data_type.end())
	{
		if (!advanceIt())
			return false;
		return true;
	}
	else if (it->getType() == "str literal")
	{
		if (!advanceIt())
			return false;
		return true;
	}
	else if (it->getType() == identifier)
	{
		if (!pointDataFile->symbolTable.varIsInTable(it->getName(), functionScope, innerScope))
		{
			makeBooBoo("undefined variable");
			return false;
		}
		if (!advanceIt())
			return false;
		return true;
	}
	else if (it->getName() == "true" || it->getName() == "false")
	{
		if (!advanceIt())
			return false;
		return true;
	}	
	else
	{
		makeBooBoo("unrecognized value");
		return false;
	}
}

/**************************************************************
*	  Purpose:	clears the spaces in the token vector to the next non-space token.
*
*         Entry:  Requires a current vector of cToken objects are
*			already loaded into the class.
*
*          Exit:  Returns a bool value to indicate the results of
*				the method's validity tests.
****************************************************************/
int cCompileCheck::clearSpaces(bool notEnd = false)
{
	if (it == parsedFile.end() && notEnd)
	{
		makeBooBoo("Unexpected end of file reached");
		return -1;
	}
	int result = 0;

	while (it != parsedFile.end() && (it->getType() == spaces || it->getType() == rtrn))
	{
		result = 1;
		if (it->getType() == rtrn)
			lineNum++;
		it++;
		if (it == parsedFile.end() && notEnd)
		{
			makeBooBoo("Unexpected end of file reached");
			result = -1;
		}
	}
	return result;
}

/**************************************************************
*	  Purpose:	advances the token vector and checks for an unexpected end of file.
*
*         Entry:  Requires a current vector of cToken objects are
*			already loaded into the class.
*
*          Exit:  Returns a bool value to indicate the results of
*				the method's validity tests.
****************************************************************/
bool cCompileCheck::advanceIt()
{
	it++;
	if (clearSpaces(true) == -1)
		return false;
	else
		return true;
}

/**************************************************************
*	  Purpose:	updates the error message to the most immediate error.
*
*         Entry:  Requires a current vector of cToken objects are
*			already loaded into the class.
*
*          Exit:  Returns a bool value to indicate the results of
*				the method's validity tests.
****************************************************************/
bool cCompileCheck::makeBooBoo(std::string message)
{
	if (errorMessageTemp == errorMessage)
	{
		errorMessage = message;
		return true;
	}
	else
		return false;
}



/**************************************************************
*	  Purpose:	tests to see if a variable name is in the variable list.
*
*         Entry:  Requires a current vector of cToken objects are
*			already loaded into the class.
*
*          Exit:  Returns a bool value to indicate the results of
*				the method's validity tests.
****************************************************************/
std::string cCompileCheck::isVar(std::string varName)
{
	// code to test variable name
	return "";
}

/**************************************************************
*	  Purpose:	tests to see if a variable name is a data type.
*
*         Entry:  Uses the current position in the parsed file vector
*
*          Exit:  Returns a bool value to indicate the results of
*				the method's validity tests.
****************************************************************/
bool cCompileCheck::isDataType()
{
	std::hash_set<std::string>::iterator hashIt = data_type.find(it->getName());
	if (hashIt != data_type.end())
		return true;
	else
		return false;
}