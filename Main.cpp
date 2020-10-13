#include <iostream>
#include <fstream>
#include <vector>
#include <iterator>
#include <map>





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

/*
=====================================================================================================

								   UPDATE FILE PATH HERE
=====================================================================================================
*/

string filePathInput = "C:/Users/17142/Desktop/CPSC323_Assignment2_Pia Wetzel/CPSC323_Assignment2_Pia Wetzel/Test Cases/inputTest1.txt";
string filePathOutput = "C:/Users/17142/Desktop/CPSC323_Assignment2_Pia Wetzel/CPSC323_Assignment2_Pia Wetzel/Test Cases/outputText.txt";

/*
=====================================================================================================
*/

int lexer(vector<charCont> container);
void setGrammar();

int main()
{

	//initialize grammar in parser
	setGrammar();
	string filename = filePathInput;

	///containers to store chars
	vector<charCont> container;
	vector<charCont> buffer;

	ifstream code;
	code.clear();
	char c;

	int line = 1;

	//open text file containing code
	code.open(filename);


	//read character by character
	if (code.is_open())
	{
		bool comment = false;

		while (code.get(c))
		{

			//line break detected
			if (c == '\n')
			{
				line++;
			}

			//clear buffer when exclamation mark was found
			if (c == '!')
			{
				buffer.clear();
				comment = !comment;
			}

			//if an exclamation mark was detected, add characters to buffer
			if (comment)
			{
				charCont curr;
				curr.myChar = c;
				curr.lineNumber = line;

				buffer.push_back(curr);
			}

			//if no exclamation mark was detected, add characters to container
			if (comment == false && c != '!')
			{
				charCont curr;
				curr.myChar = c;
				curr.lineNumber = line;

				container.push_back(curr);

			}

		}
		//if size of buffer > 0, only one exclamation mark was found
		if (buffer.size() > 0)
		{
			//add content of buffer to container
			container.insert(container.end(), buffer.begin(), buffer.end());

		}

		charCont curr;
		curr.myChar = ' ';
		curr.lineNumber = line;
		container.push_back(curr);
		code.close();

	}


	lexer(container);



}

string getOutputPath()
{
	return filePathOutput;
}