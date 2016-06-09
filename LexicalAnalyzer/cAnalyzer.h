/************************************************************************
* Class: cAnalyzer
*
* Constructors:
*	cAnalyzer()
*
* Mutators:
*	NA
*
* Methods:
*   std::vector<cToken> analyze(std::vector<char> const &file)
*		This method takes a char vector, assumed to be the individual char characters
*		from a C program text file, parses and identifies the program tokens, and returns
*		a cToken vector listing the tokens with their types in order
*************************************************************************/

#pragma once
#include <vector>
#include "cToken.h"
#include <set>
#include <string>
#include <iostream>
#include <unordered_set>
#include <Hash_Set>

class cAnalyzer
{
public:
	// default constructor
	cAnalyzer();

	// default deconstructor
	~cAnalyzer();

	// parses the file parameter into tokens and their types
	// and returns them as a vector of cToken objects
	std::vector<cToken> analyze(std::vector<char> const &file);

private:
	std::vector<char>::const_iterator it;
	std::vector<char>::const_iterator start;
	std::hash_set<char> symbols;
	std::hash_set<char> operators;
	std::hash_set<std::string> preprocessor;
	std::hash_set<const std::string> keywords;
	std::vector<cToken> result;
	std::string error = "ERROR - Unknown or incorrect type!";

	// used in conjuction with std::vector<cToken> analyze(std::vector<char> const &file)
	// to parse the sections of file that are numbers
	int number(std::vector<char> const &file);

	// used in conjuction with std::vector<cToken> analyze(std::vector<char> const &file)
	// to parse the sections of file that are string literals
	int stringLiteral(std::vector<char> const &file);

	// used in conjuction with std::vector<cToken> analyze(std::vector<char> const &file)
	// to parse the sections of file that are identifiers
	int identifier(std::vector<char> const &file);

	// used in conjuction with std::vector<cToken> analyze(std::vector<char> const &file)
	// to parse the sections of file that are operators
	int isOperator(std::vector<char> const &file);
};

