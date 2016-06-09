#include "stdafx.h"
#include "cAnalyzer.h"

/**************************************************************
*	  Purpose:  Constructs a cAnalyzer object, and builds lists
*				for identifying symbols, operators, and keywords.
*
*         Entry:  NA
*
*          Exit:  Returns a cAnalyzer object
****************************************************************/
cAnalyzer::cAnalyzer()
{
	symbols.insert('(');
	symbols.insert(')');
	symbols.insert('{');
	symbols.insert('}');
	symbols.insert(';');
	symbols.insert('.');
	symbols.insert(',');
	symbols.insert('[');
	symbols.insert(']');

	operators.insert('=');
	operators.insert('*');
	operators.insert('+');
	operators.insert('-');
	operators.insert('/');
	operators.insert('<');
	operators.insert('>');
	operators.insert('!');
	operators.insert('%');
	operators.insert('~');
	operators.insert('^');
	operators.insert('&');
	operators.insert('|');
	operators.insert('?');

	preprocessor.insert("#ifdef");
	preprocessor.insert("#ifndef");
	preprocessor.insert("#if");
	preprocessor.insert("#endif");
	preprocessor.insert("#else");
	preprocessor.insert("#elif");
	preprocessor.insert("#define");
	preprocessor.insert("#include");

	keywords.insert("auto");
	keywords.insert("bool");
	keywords.insert("break");
	keywords.insert("case");
	keywords.insert("char");
	keywords.insert("const");
	keywords.insert("continue");
	keywords.insert("default");
	keywords.insert("do");
	keywords.insert("double");
	keywords.insert("else");
	keywords.insert("enum");
	keywords.insert("extern");
	keywords.insert("false");
	keywords.insert("float");
	keywords.insert("for");
	keywords.insert("goto");
	keywords.insert("if");
	keywords.insert("int");
	keywords.insert("long");
	keywords.insert("printf");
	keywords.insert("register");
	keywords.insert("return");
	keywords.insert("scanf");
	keywords.insert("short");
	keywords.insert("signed");
	keywords.insert("sizeof");
	keywords.insert("static");
	keywords.insert("string");
	keywords.insert("struct");
	keywords.insert("switch");
	keywords.insert("true");
	keywords.insert("typedef");
	keywords.insert("union");
	keywords.insert("unsigned");
	keywords.insert("void");
	keywords.insert("volatile");
	keywords.insert("while");
}

/**************************************************************
*	  Purpose:  Default deconstructor
*
*         Entry:  NA
*
*          Exit:  NA
****************************************************************/
cAnalyzer::~cAnalyzer()
{
}

/**************************************************************
*	  Purpose:  Organizes the identification of tokens and types
*				by branching program into different directions,
*				utilyzing various other methods as needed,
*				depending on the first character of a subset,
*				places the token and type into a cToken object
*				and adds the cToken object to a vector.
*
*         Entry:  Method takes a vector of char of C text by reference.
*
*          Exit:  Returns a vector of cToken objects in the same
*					order that they appeared in the file vector
****************************************************************/
std::vector<cToken> cAnalyzer::analyze(std::vector<char> const &file)
{
	result.clear();
	it = file.begin();

	while (it != file.end())
	{
		if (*it == '\n')
		{
			result.push_back(cToken("RETURN", "Crg Rtrn"));
			it++;
		}
		else if (isspace(*it))
		{
			while (it != file.end() && isspace(*it) && *it != '\n')
			{
				it++;
			}
			result.push_back(cToken("SPACES", "spaces"));
		}
		else if (isdigit(*it) || *it == '.')
		{
			if (number(file) < 0)
				return result;
		}
		else if (isalpha(*it) || *it == '_')
		{
			if (identifier(file) < 0)
				return result;
		}
		else if (*it == '\"' || *it == '\'')
		{
			if (stringLiteral(file) < 0)
				return result;
		}
		else if (*it == '#')
		{
			std::string temp = std::string(it, it + 1);
			result.push_back(cToken(temp, "preprocessor"));
			it++;
		}
		else if (std::find(symbols.begin(), symbols.end(), *it) != symbols.end())
		{
			std::string temp = std::string(it, it + 1);
			result.push_back(cToken(temp, "symbol"));
			it++;
		}
		else if (std::find(operators.begin(), operators.end(), *it) != operators.end())
		{
			if (isOperator(file) < 0)
				return result;
		}
		else
		{
			std::string temp = std::string(it, it + 1);
			result.push_back(cToken(temp, error));
			it++;
		}
	}	

	return result;
}

/**************************************************************
*	  Purpose:  Takes a char vector by reference, and
*				uses the cAnalyzer object iterator to identify 
*				the current position, and finds the current token
*				in the file vector, identifies it as a number,
*				organizes it into a cToken obect and adds it into
*				the cToken vector.
*
*         Entry:  Method takes a vector of char of C text by reference,
*					and uses the cAnalyzer char vector iterator.
*
*          Exit:  Modifies the cAnalyzer char vector iterator
*					by advancing the iterator as needed,
*					and adds a cToken object to the cToken result vector.
****************************************************************/
int cAnalyzer::number(std::vector<char> const  &file)
{
	std::string type = "integer";
	start = it;
	
	int state = 0;
	int errorState = 0;

	while (isdigit(*it) || *it == '.')
	{
		if (*it == '.')
		{
			if (!state)
			{
				state = 1;
				type = "float";
			}
			else
			{
				type = error;
				errorState = 1;
			}
		}
		it++;
	}

	if (*it == 'l' && !state && !errorState)
	{
		type = "long";
		it++;
	}
	else if (*it == 'd' && !errorState)
	{
		type = "double";
		it++;
	}
	else if (*it == 'f' && !errorState)
	{
		type = "float";
		it++;
	}

	if (isalpha(*it) || *it == '_')
	{
		type = error;
		while (it != file.end() && isalpha(*it) || *it == '_')
		{
			it++;
		}
	}

	std::string temp = std::string(start, it);
	result.push_back(cToken(temp, type));
	return 0;
}

/**************************************************************
*	  Purpose:  Takes a char vector by reference, and
*				uses the cAnalyzer object iterator to identify
*				the current position, and finds the current token
*				in the file vector, identifies it as a string literal,
*				organizes it into a cToken obect and adds it into
*				the cToken vector.
*
*         Entry:  Method takes a vector of char of C text by reference,
*					and uses the cAnalyzer char vector iterator.
*
*          Exit:  Modifies the cAnalyzer char vector iterator
*					by advancing the iterator as needed,
*					and adds a cToken object to the cToken result vector.
****************************************************************/
int cAnalyzer::stringLiteral(std::vector<char> const  &file)
{
	std::string type = "str literal";
	start = it;
	char previous = *it;

	if (*it == '\"')
	{
		it++;
		while (it != file.end() && *it != '\"' || previous == '\\')
		{
			if (it == file.end())
			{
				type = error;
				break;
			}
			previous = *it;
			it++;
		}
	}
	else
	{
		it++;
		while (it != file.end() && *it != '\'' || previous == '\\')
		{
			if (it == file.end())
			{
				type = error;
				break;
			}
			previous = *it;
			it++;
		}
	}
		
	it++;
	std::string temp = std::string(start, it);
	result.push_back(cToken(temp, type));
	return 0;
}

/**************************************************************
*	  Purpose:  Takes a char vector by reference, and
*				uses the cAnalyzer object iterator to identify
*				the current position, and finds the current token
*				in the file vector, identifies it as an identifier,
*				organizes it into a cToken obect and adds it into
*				the cToken vector.
*
*         Entry:  Method takes a vector of char of C text by reference,
*					and uses the cAnalyzer char vector iterator.
*
*          Exit:  Modifies the cAnalyzer char vector iterator
*					by advancing the iterator as needed,
*					and adds a cToken object to the cToken result vector.
****************************************************************/
int cAnalyzer::identifier(std::vector<char> const  &file)
{
	std::string type = "identifier";
	start = it;
	while (it != file.end() && isalpha(*it) || isdigit(*it) || *it == '_')
	{
		it++;
	}
	std::string temp = std::string(start, it);
	std::hash_set<std::string>::iterator hashIt = keywords.find(temp);
	if (hashIt != keywords.end())
		type = "keyword";
	result.push_back(cToken(temp, type));
	return 0;
}

/**************************************************************
*	  Purpose:  Takes a char vector by reference, and
*				uses the cAnalyzer object iterator to identify
*				the current position, and finds the current token
*				in the file vector, identifies it as an operator,
*				organizes it into a cToken obect and adds it into
*				the cToken vector.
*
*         Entry:  Method takes a vector of char of C text by reference,
*					and uses the cAnalyzer char vector iterator.
*
*          Exit:  Modifies the cAnalyzer char vector iterator
*					by advancing the iterator as needed,
*					and adds a cToken object to the cToken result vector.
****************************************************************/
int cAnalyzer::isOperator(std::vector<char> const  &file)
{
	std::string type = "operator";
	int state = 0; // used to track path through graph - value of unnlucky 1313 indicates error, 1314 and 1315 indicate temporary errors
	start = it;
	
	while (it != file.end() && std::find(operators.begin(), operators.end(), *it) != operators.end())
	{
		if (*it == '+')
		{
			switch (state)
			{
			case 0:
				state = 1;
				break;
			case 1:
				state = 10;
				break;
			case 10:
				state = 1314;
				break;
			case 1314:
				state = 10;
				break;
			default:
				state = 1313;
			}
		}
		else if (*it == '-')
		{
			switch (state)
			{
			case 0:
				state = 2;
				break;
			case 2:
				state = 11;
				break;
			case 11:
				state = 1315;
				break;
			case 1315:
				state = 11;
				break;
			default:
				state = 1313;
			}
		}
		else if (*it == '*')
		{
			state = (state == 0) ? 3 : 1313;
		}
		else if (*it == '/')
		{
			if (state == 4)
			{
				it--;
				if (it > start)
				{
					std::string temp = std::string(start, it);
					result.push_back(cToken(temp, type));
				}
				state = 1212; // special circumstance - this is a comment!
				break;
			}				
			else
				state = (state == 0) ? 4 : 1313;
		}
		else if (*it == '%')
		{
			state = (state == 0) ? 5 : 1313;
		}
		else if (*it == '|')
		{
			state = (state == 0 || state == 6) ? 6 : 1313;
		}
		else if (*it == '&')
		{
			state = (state == 0 || state == 7) ? 7 : 1313;
		}
		else if (*it == '^')
		{
			state = (state == 0) ? 8 : 1313;
		}
		else if (*it == '=')
		{
			if (state == 0)
				state = 9;
			else if (state < 9)
				state = 12;
			else if (state == 9)
				state = 13;
			else
				state = 1313;
		}
		it++;
	}

	if (state == 1313 || state == 1314 || state == 1315)
		type = error;
	if (state == 1212)
	{
		type = "comment";
		while (it != file.end() && *it != '\n')
		{
			it++;
		}
	}

	std::string temp = std::string(start, it);
	if (state != 1212)
		result.push_back(cToken(temp, type));
	return 0;
}