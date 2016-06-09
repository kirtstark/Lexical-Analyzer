/***********************************************************
* Author:				Kirt Stark
* Date Created:			11/27/15
* Last Modification Date:	2/22/16
* Lab Number:			CST 229 Lab
* Filename:				LexicalAnalyzer.cpp
*
* Overview:
*   This program reads in c language programs as text files,
*   parses the text and identifies each token by type,
*   and stores the tokens and their types in a vector for future processing.
*   Next it processes the preprocessor define and undefine statements
*   and removes any other preprocessor statement.
*   It currently only processes define statements with a single replacement token.
*
* Input:
*	The input will consist of a series of text files stored in 
*   The /TextFiles directory. Each will be read in automatically 
*   as identified within the program. The programs are expected to 
*   be C language text files.
*
* Output:
*   The completed vector of tokens with the associated type name
*   will be output to the screen for each text file listed. The 
*   output will be in the form:
*
*   type			token
* 
*   Example:
*   
*   identifier		foo
*   operator		++
*   str literal		"hello world!"
*   crg return		RETURN
*
*
*  After this, the parsed file is processed for the preprocessor
*  statements and the final result is printed to the screen
*  in the same format as above. The program is partially compiled
*  and the symbol table is printed to the screen.
************************************************************/

#include "stdafx.h"
#include <conio.h>
#include <iostream>
#include <string>
#include <vector>
#include "fileAccess.h"
#include "cFileData.h"
#include "cToken.h"
#include "cAnalyzer.h"
#include "cCompileCheck.h"
#include "cPreprocessor.h"

// Provides Any Key functionality
inline void PressAnyKey();

// Prints the standard user instructions to the screen
void Instructions();

// Loads the disk file names into a vector 
inline std::vector<std::string> ReadInFileNames();

// Translates all non-ASCII code char into ASCII code
void TranslateToAscii(std::vector<char>& dataFile);

int main()
{
	std::vector<cFileData> fileDatas;
	//std::vector<std::vector<cToken>> parsedFiles;
	Instructions();	
	std::vector<std::string> files = ReadInFileNames();
	
	for each(std::string name in files)
	{
		fileDatas.push_back(cFileData(name));
	}

	cAnalyzer newAnalyzer;

	// while loop through file names
	for (std::vector<cFileData>::iterator itFiles = fileDatas.begin(); itFiles != fileDatas.end(); ++itFiles)
	{
		fileAccess* data1 = new fileAccess(itFiles->fileName);
		std::vector<char> dataFile = data1->readEntireFile();
		TranslateToAscii(dataFile);
		itFiles->parsedFile = newAnalyzer.analyze(dataFile);
		
		delete data1;
	}

	// this section displays the parsed files prior to further processing
	/*
	for each (cFileData cfd in fileDatas)
	{
		std::cout << "\n\n\nThis is results for file " << cfd.fileName << std::endl;
		std::cout << "    =========================================================================\n";
		cToken::tableTitles();
		for each (cToken ct in cfd.parsedFile)
		{
			ct.display();
		}
		PressAnyKey();
	}
	*/

	// send parsed files through the preprocessor
	cPreprocessor preprocessor;
	
	for (int i = 0; i < fileDatas.size(); i++)
	{
		preprocessor.process(fileDatas[i]);
	}

	PressAnyKey();
	
	// this section displays the parsed files following the preprocessor
	/*  
	int count = 0;
	for each (cFileData fd in fileDatas)
	{
		std::cout << std::endl << std::endl << "For file #" << ++count << std::endl << "===========================" << std::endl;
		for each(cToken ct in fd.parsedFile)
		{
			ct.display();
		}
		std::cout << std::endl << std::endl;
		PressAnyKey();
	}
	*/
	
	// This section displays the results of program compilation, with errors where applicable
	cCompileCheck ccc;
	for (int i = 0; i < fileDatas.size(); i++)
	{
		if (i < files.size())
			std::cout << "    Results for file: " << fileDatas[i].fileName << std::endl;
		else
			std::cout << "        CANNOT ACCESS FILE NAME!!!!" << std::endl;
		if (ccc.programCheck(fileDatas[i]))
			std::cout << "File was okay" << std::endl;
		else
			std::cout << "Error, file was not good" << std::endl;

		std::cout << std::endl << std::endl;
	}

	std::cout << "Final symbol tables:" << std::endl << std::endl;
	PressAnyKey();

	// This section displays the symbol table for each of the programs after compilation
	for (int i = 0; i < fileDatas.size(); i++)
	{
		std::cout << "For file " << fileDatas[i].fileName << std::endl << std::endl;
		fileDatas[i].symbolTable.display();
	}

	std::cout << "Final statement list:" << std::endl << std::endl;
	PressAnyKey();

	// This section displays the statement list for each of the programs after compilation
	for (int i = 0; i < fileDatas.size(); i++)
	{
		std::cout << "\n\nList of statments for file " << fileDatas[i].fileName << std::endl << std::endl;
		for each (std::string statement in fileDatas[i].statements)
		{
			std::cout << statement << std::endl;
		}
	}


	// End of program dialog
	std::cout << "That is the end of our list. Thank you for playing." << std::endl << std::endl;
	PressAnyKey();

	return 0;
}

/**************************************************************
*	  Purpose:  Provides a reliable, standard Press Any Key method
*
*         Entry:  none
*
*          Exit:  Clears the keyboard input buffer, then waits until
*		      user presses another key
*                 Again clears the keyboard input buffer
****************************************************************/

void PressAnyKey()
{
	while (_kbhit())
	{
		_getch();
	}
	std::cout << std::endl << std::endl << "Press any key to continue" << std::endl << std::endl;	
	std::cin.clear();
	
	while (1)
	{
		if (_kbhit())
		{
			while (_kbhit())
			{
				_getch();
			}
			break;
		}
	}
}

/**************************************************************
*	  Purpose:  Provides user instructions
*
*         Entry:  none
*
*          Exit:  Prints the instructions to the screen, calls PressAnyKey()
				and clears the screen
****************************************************************/
inline void Instructions()
{
	std::cout << "Included in this project are several test files of the c programming language." << std::endl;
	std::cout << "They are all located in the project directory within the TextFiles folder." << std::endl;
	std::cout << "The names for each of these files is read into the program automatically," << std::endl;
	std::cout << "so there is no need for you to enter that information in." << std::endl;
	std::cout << "Instead, you will be prompted to press any key and " << std::endl;
	std::cout << "a new file or step will be processed each time until all files are completed." << std::endl;
	PressAnyKey();
	system("cls");
}

/**************************************************************
*	  Purpose:  Stores the text file names into a vector
*
*         Entry:  File names being added should be the names
*			of the files in the TextFiles folder
*
*          Exit:  Returns a vector listing the file names
*		      that are to be used by the program to read in 
*                 C-code text files
****************************************************************/
inline std::vector<std::string> ReadInFileNames()
{
	std::vector<std::string> files;
	files.push_back("TextFiles/testA.txt");
	files.push_back("TextFiles/testB.txt");
	//files.push_back("TextFiles/testC.txt");
	//files.push_back("TextFiles/testA1.txt");
	//files.push_back("TextFiles/testB1.txt");
	//files.push_back("TextFiles/testB2.txt");

	return files;
}

/**************************************************************
*	  Purpose:  Changes Unicode chars to ASCII code chars
*
*         Entry:  dataFile is a char vector read in from C-code text files
*
*          Exit:  Each char in the vector is examined, and
*		      those that are not in ASCII code are modified
*                 to be ASCII code.
*					dataFile may be modified.
****************************************************************/
void TranslateToAscii(std::vector<char>& dataFile)
{
		for (int i = 0; i < dataFile.size(); i++)
		{
			if (int(dataFile[i]) < 1)
			{
				if (int(dataFile[i]) == -108 || int(dataFile[i]) == -109)
				{
					dataFile[i] = '\"';
				}

				if (int(dataFile[i]) == -110 || int(dataFile[i]) == -111)
				{
					dataFile[i] = '\'';
				}

				else if (int(dataFile[i]) == -106)
				{
					dataFile[i] = '-';
				}

				else if (int(dataFile[i]) < 0)
				{
					dataFile[i] = ' ';
				}
			}

		}
	
}
