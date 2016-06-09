/************************************************************************
* Class: cToken
*
* Constructors:
*	cToken()
*		name and type are initialized to null
*	cToken(std::string _name, std::string _type)
*		the parameters are used to initialize the values for name and type
* Mutators:
*	void setName(std::string _name)
*		sets or changes the name value
*	void setType(std::string _type)
*		sets or changes the type value
* Methods:
*	void setName(std::string _name)
*		sets or changes the name value
*	std::string getName()
*		returns the value in name as a string
*	void setType(std::string _type)
*		sets or changes the type value
*	std::string getType()
*		returns the value in type as a string
*	void display()
*		displays the token object to the screen in the form of type      name
*	void tableTitles()
*		displays the title bar for the table
*************************************************************************/

#pragma once
#include <string>

class cToken
{
public:
	// default constructor
	cToken();

	// constructor, name and type are initialized from parameters
	cToken(std::string _name, std::string _type);

	// default deconstructor
	~cToken();


	// sets the name
	void setName(std::string _name);

	// returns the current value in name
	std::string getName();

	// sets the type
	void setType(std::string _type);

	// returns the current value in type
	std::string getType();

	// displays a token to the screen as type and name
	void display();

	// displays the table header for the token display
	static void tableTitles();

private:
	std::string name;
	std::string type;
};

