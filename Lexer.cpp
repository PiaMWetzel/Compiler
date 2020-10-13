
/*======================================================

	Name: Pia Wetzel
	CPSC 323
	Assignment 1
	September 28, 2019

PLEASE INPUT OR UPDATE FILE PATH/NAME IN MAIN METHOD.

========================================================
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <iterator>
#include <map>
#include <string>
using namespace std;



struct charCont
{
	char myChar;
	int lineNumber;
};

struct ToLexCont
{
	string token;
	string lexeme;
	int lineNumber;
};



string parser(ToLexCont toLex);
vector<string> getOutputFile();
string getOutputPath();

//adding Tokens/Lexemes to vector to be printed later
void addToFile(string token, string lexeme, vector<string> someVector)
{
	someVector.push_back(token + "\t=\t" + lexeme);
}

//checks if char is an operator
bool checkIfOperator(char someChar, vector<char> someVector)
{
	for (char items : someVector)
	{
		if (items == someChar)

			//operator found
			return true;
	}

	//no operator
	return false;
}

//checks if char is a separator
bool checkIfSeperator(char someChar, vector<char> someVector)
{
	for (char items : someVector)
	{
		if (items == someChar)
			//separator found
			return true;
	}

	//no separator
	return false;
}

//checks if string is a keyword
bool checkIfKeyword(string someString, vector<string> someVector)
{
	for (string items : someVector)
	{
		if (items == someString)

			//keyword found
			return true;
	}

	//no keyword
	return false;
}

//checks if char is a letter
bool checkIfLetter(char someChar, string someString)
{
	int pos = someString.find(someChar);
	return pos >= 0;

}

//checks if char is a number
bool checkIfNumber(char someChar, string someString)
{
	int pos = someString.find(someChar);
	return pos >= 0;

}


//write tokens/lexemes to file and display on console
void writeToFile(vector<string> someString)
{

	fstream output;
	output.open(getOutputPath(), fstream::out);
	if (output)
	{
		for (string items : someString)
		{
			cout << items << endl;
			//cout << "Hello"<< endl;
			output << items << endl;
		}
		output.flush();
		output.close();
	}
	else
	{
		cout << "Error creating output file " << endl;
	}

}



//lexer method takes care of the lexical analysis
int lexer(vector<charCont> container)
{

	//Containers holding keyword, separator, operator values


	vector<string> outputFile;
	outputFile.push_back("\n\nTOKENS\t\t\t\t\tLEXEMES\n");

	vector<string> keywords = { "int", "float", "bool", "if", "else", "then", "endif", "while", "whileend", "do",
								"doend", "for", "forend", "input", "output", "and", "or", "function", "true", "false" };
	string letters = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	string numbers = "1234567890";
	vector<char> seperators = { '\'', '(', ')', '{', '}', '[', ']', ',', '.', ':', ';', '!' };
	vector<char> operators = { '*','+', '-', '=', '/', '>', '<', '%' };

	//start state is 1
	int currentState = 1;
	string lexeme = "";
	int pos = 0;
	int currentLexemeBegin = 0;
	bool added = false;

	ToLexCont toLex;

	//loop through container containing all characters
	while (pos < container.size() - 1)
	{
		//update the start index of the new lexeme after the previous has been added to the output file
		if (added)
		{
			lexeme = "";
			currentLexemeBegin = pos;
			added = false;
		}

		if (lexeme == "%%")
		{

			outputFile.push_back("OPERATOR\t\t\t=\t\t" + lexeme);
			toLex.token = "OPERATOR";
			toLex.lexeme = "%%";

			parser(toLex);
			lexeme = "";
			currentState = 1;

		}

		switch (currentState)
		{

			//FIRST STATE
		case 1:
		{
			//terminate
			if ((container.at(pos)).myChar == '$')
			{
				lexeme += (container.at(pos)).myChar;
				currentState = 5;


			}

			//See if current char is a letter
			if (checkIfLetter((container.at(pos)).myChar, letters))
			{
				lexeme += (container.at(pos)).myChar;
				if (!checkIfLetter((container.at(pos + 1)).myChar, letters + "$") && !checkIfNumber((container.at(pos + 1)).myChar, numbers))
				{
					outputFile.push_back("IDENTIFIER\t\t=\t\t" + lexeme);

					toLex.lexeme = lexeme;
					toLex.token = "IDENTIFIER";
					toLex.lineNumber = (container.at(pos)).lineNumber;

					//Parser
					string ret = parser(toLex);
					if (ret == "ERROR")
					{
						writeToFile(getOutputFile());
						return 0;
					}

					currentState = 1;
					added = true;
				}
				else
				{
					//if next char is also a letter, move to state 2
					currentState = 2;
				}


			}
			//check if current char is a separator
			else if (checkIfSeperator((container.at(pos)).myChar, seperators))
			{
				lexeme += (container.at(pos)).myChar;

				outputFile.push_back("SEPARATOR\t\t=\t\t" + lexeme);
				currentState = 1;
				added = true;

				toLex.lexeme = lexeme;
				toLex.token = "SEPARATOR";
				toLex.lineNumber = (container.at(pos)).lineNumber;

				//Parser
				string ret = parser(toLex);
				if (ret == "ERROR")
				{
					writeToFile(getOutputFile());
					return 0;
				}

			}

			//check if current char is an operator
			else if (checkIfOperator((container.at(pos)).myChar, operators))
			{
				lexeme += (container.at(pos)).myChar;

				//outputFile.push_back("OPERATOR\t\t=\t\t" + lexeme);

				if (lexeme != "%")
				{
					currentState = 1;
					added = true;

					toLex.lexeme = lexeme;
					toLex.token = "OPERATOR";
					toLex.lineNumber = (container.at(pos)).lineNumber;

					//Parser
					string ret = parser(toLex);
					if (ret == "ERROR")
					{
						writeToFile(getOutputFile());
						return 0;
					}

				}
			}

			//check if current char is a number
			else if (checkIfNumber((container.at(pos)).myChar, numbers))
			{
				lexeme += (container.at(pos)).myChar;
				//if next char is also a number, go to state 3
				if (checkIfNumber((container.at(pos + 1)).myChar, numbers))
				{
					currentState = 3;
				}
				//if next char is a letter, go to state 5 (terminate, invalid token)
				else if (checkIfLetter((container.at(pos + 1)).myChar, letters))
				{
					currentState = 5;
				}
				//if next char is a ., go to state 4 (real number)
				else if ((container.at(pos)).myChar == '.')
				{
					currentState = 4;
				}
				//otherwise it's a single-digit decimal
				else
				{
					outputFile.push_back("DECIMAL\t\t\t=\t\t" + lexeme);
					currentState = 1;
					added = true;

					toLex.lexeme = lexeme;
					toLex.token = "DECIMAL";
					toLex.lineNumber = (container.at(pos)).lineNumber;

					//Parser
					string ret = parser(toLex);
					if (ret == "ERROR")
					{
						writeToFile(getOutputFile());
						return 0;
					}
				}
			}


			break;

		}

		case 2:
		{
			//check if current char is a letter, number, or $
			if (checkIfLetter((container.at(pos)).myChar, letters + "$")
				|| checkIfNumber((container.at(pos)).myChar, numbers))
			{
				lexeme += (container.at(pos)).myChar;
				currentState = 2;

				//if next char is neither a letter, nor a $, or number, we have found either a keyword or identifier
				if (!checkIfLetter((container.at(pos + 1)).myChar, letters + "$")
					&& !checkIfNumber((container.at(pos + 1)).myChar, numbers))
				{
					//check whether we found keyword or identifier
					if (checkIfKeyword(lexeme, keywords))
					{

						outputFile.push_back("KEYWORD\t\t\t=\t\t" + lexeme);
						toLex.token = "KEYWORD";

						toLex.lexeme = lexeme;
						toLex.lineNumber = (container.at(pos)).lineNumber;

						//Parser
						string ret = parser(toLex);

						if (ret == "ERROR")
						{
							writeToFile(getOutputFile());
							return 0;
						}
					}
					else
					{
						outputFile.push_back("IDENTIFIER\t\t=\t\t" + lexeme);

						toLex.lexeme = lexeme;
						toLex.token = "IDENTIFIER";
						toLex.lineNumber = (container.at(pos)).lineNumber;

						//Parser
						string ret = parser(toLex);
						if (ret == "ERROR")
						{
							writeToFile(getOutputFile());
							return 0;
						}
					}

					//go back to state 1
					currentState = 1;
					added = true;
				}

			}

			break;
		}

		//STATE 3 - NUMBERS
		case 3:
		{
			lexeme += (container.at(pos)).myChar;

			//if next char is a number, terminate
			if (checkIfLetter((container.at(pos + 1)).myChar, letters))
			{
				currentState = 5;
			}
			//if next char is neither a number nor . (period), we found a decimal
			else if (!checkIfNumber((container.at(pos + 1)).myChar, numbers) && (container.at(pos + 1)).myChar != '.')
			{
				outputFile.push_back("DECIMAL\t\t\t=\t\t" + lexeme);
				added = true;

				//go back to state 1
				currentState = 1;

				toLex.lexeme = lexeme;
				toLex.token = "DECIMAL";
				toLex.lineNumber = (container.at(pos)).lineNumber;

				//Parser
				string ret = parser(toLex);
				if (ret == "ERROR")
				{
					writeToFile(getOutputFile());
					return 0;
				}
			}
			//if next char is a . (period), we found a real number
			else if ((container.at(pos + 1)).myChar == '.')
			{
				//go to state 4
				currentState = 4;
			}

			break;
		}

		//STATE 4 - REAL NUMBERS
		case 4:
		{

			lexeme += (container.at(pos)).myChar;

			if (!checkIfNumber((container.at(pos + 1)).myChar, numbers))
			{
				outputFile.push_back("REAL\t\t\t=\t\t" + lexeme);
				currentState = 1;
				added = true;

				toLex.lexeme = lexeme;
				toLex.token = "REAL";
				toLex.lineNumber = (container.at(pos)).lineNumber;

				//Parser
				string ret = parser(toLex);
				if (ret == "ERROR")
				{
					writeToFile(getOutputFile());
					return 0;
				}
			}


			break;
		}

		//State 5 - Non-Valid Tokens
		case 5:
		{
			lexeme += (container.at(pos)).myChar;

			if (!checkIfNumber((container.at(pos + 1)).myChar, numbers) && !checkIfLetter((container.at(pos + 1)).myChar, letters + "$"))
			{
				outputFile.push_back("INVALID TOKEN\t\t=\t\t" + lexeme);
				currentState = 1;
				added = true;

				toLex.lexeme = lexeme;
				toLex.token = "INVALID TOKEN";
				toLex.lineNumber = (container.at(pos)).lineNumber;

				vector<string> opf = getOutputFile();
				opf.push_back("\n\t !!! ERROR: " + toLex.token + " " + toLex.lexeme);
				opf.push_back("\t Line number: " + to_string(toLex.lineNumber));
				writeToFile(opf);
				return 0;
			}

			break;
		}

		}

		// check next char
		pos++;
	}

	//print lexemes/tokens list
	vector<string> opf = getOutputFile();
	opf.push_back("\n\t Parsed without errors");
	writeToFile(opf);
	return 0;


}



