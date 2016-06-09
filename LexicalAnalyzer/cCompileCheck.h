#pragma once
#include <vector>
#include <Hash_Set>
#include "cToken.h"
#include "cSymbol.h"
#include "cSymbolTable.h"
#include "cFileData.h"

/************************************************************************
* Class: cCompileCheck
*
* Constructors:
*	cCompileCheck();
* Mutators:
*	NA
* Methods:
*		bool programCheck(std::vector<cToken>& parsedFile);
*			public method, takes the cToken vector, updates the variables, and 
*			calls the program() method.
*		bool program();
*			beginning point, calls all other programs and processes
*			through entire vector of tokens to determine any
*			syntax errors, returning first error found or that file is good.
*		bool function_header();
*			tests any and all function headers for syntax errors.
*		bool function_body();
*			tests the function bodies for syntax errors.
*		bool parameter_list();
*			tests the parameter lists for syntax errors.
*		bool var_list();
*			tests the variable lists for syntax errors.
*		bool exp();
*			tests an expression, including a list of expressions, for sytax errors.
*		bool value();
*			tests if a token is a value.
*		bool op_list();
*			tests a list of operators to see if it is all operators.
*		bool expression_list();
*			tests a list of expressions for syntax errors.
*		bool expression();
*			tests an individual expression for syntax errors.
*		bool statement_list();
*			loops through a list of statements and tests for syntax errors.
*		bool statement();
*			tests an individual statement for syntax errors.
*		bool value_list();
*			tests a list of values for syntax errors.
*		bool f_call();
*			test if it has a function call and tests  for syntax errors
*		bool ex_list_x();
*			an extension and helper function for expression_list to avoid left recursion.
*		bool string_list();
*			tests to determine if it is a list of strings.
*		bool comp_stat();
*			tests for a comparrision and for syntax errors.
*		bool comp_statements();
*			tests for syntax error throughout an entire comparrisonn statement.
*		bool advanceIt();
*			advances the token vector and checks for an unexpected end of file.
*		int clearSpaces(bool notEnd);
*			clears the spaces in the token vector to the next non-space token.
*		bool makeBooBoo(std::string message);
*			updates the error message to the most immediate error.
*		bool isFunction(std::string funcName);
*			tests to see if a function name is in the function list.
*		std::string isVar(std::string varName);
*			tests to see if a variable name is in the variable list.
*************************************************************************/

class cCompileCheck
{
public:
	// default constructor
	cCompileCheck();

	// default deconstructor
	~cCompileCheck();

	bool programCheck(cFileData& parsedFile);
	

private:
	int lineNum = 1;
	std::string identifier = "identifier";
	std::string keyword = "keyword";
	std::string rtrn = "Crg Rtrn";
	std::string spaces = "spaces";
	std::string anOperator = "operator";
	std::string errorMessage = "unknown error";
	std::string errorMessageTemp = "";
	std::string thisStatementType = "";
	std::vector<cToken> parsedFile;
	std::vector<cToken>::iterator it;
	std::string functionScope = "";
	int innerScope = 0;
	cFileData* pointDataFile;
	std::string varType;
	std::vector<std::string> varList;

	std::hash_set<std::string> data_type;

	/*beginning point, calls all other programs and processes
		through entire vector of tokens to determine any
		syntax errors, returning first error found or that file is good.*/
	bool program();

	// tests any and all function headers for syntax errors.
	bool function_header();

	// tests the function bodies for syntax errors.
	bool function_body();

	// tests the parameter lists for syntax errors.
	bool parameter_list();

	// tests the variable lists for syntax errors.
	bool var_list();

	// tests an expression, including a list of expressions, for sytax errors.
	bool exp();

	// tests if a token is a value.
	bool value();

	// tests a list of operators to see if it is all operators.
	bool op_list();

	// tests a list of expressions for syntax errors.
	bool expression_list();

	// tests an individual expression for syntax errors.
	bool expression();

	// loops through a list of statements and tests for syntax errors.
	bool statement_list();

	// tests an individual statement for syntax errors.
	bool statement();

	// tests a list of values for syntax errors.
	bool value_list();

	// test if it has a function call and tests  for syntax errors
	bool f_call();

	// an extension and helper function for expression_list to avoid left recursion.
	bool ex_list_x();

	// tests to determine if it is a list of strings.
	bool string_list();

	// tests for a comparrision and for syntax errors.
	bool comp_stat();

	// tests for syntax error throughout an entire comparrisonn statement.
	bool comp_statements();

	// advances the token vector and checks for an unexpected end of file.
	bool advanceIt();

	// clears the spaces in the token vector to the next non-space token.
	int clearSpaces(bool notEnd);

	// updates the error message to the most immediate error.
	bool makeBooBoo(std::string message);


	// tests to see if a variable name is in the variable list.
	std::string isVar(std::string varName);

	// test to see if the current token is a data type
	bool isDataType();
};

