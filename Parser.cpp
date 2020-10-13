
/*======================================================

	Name: Pia Wetzel
	CPSC 323
	Assignment 2
	November 13, 2019

PLEASE INPUT OR UPDATE FILE PATH/NAME IN MAIN METHOD.

========================================================
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <iterator>
#include <map>
#include <stack>
#include <string>


using namespace std;

//Parse table
string pt[40][42];
//Grammar rules for look up
map<string, string> grammar;

stack<string> myStack;

stack<string> tempStack;

vector<string> output;

//struct containing token/lexeme pair with associated line number
struct ToLexCont
{
	string token;
	string lexeme;
	int lineNumber;
};



//checks if given input string is a terminal
bool checkIfTerminal(string s)
{
	for (int i = 0; i < 42; i++)
	{

		if (pt[0][i] == s)
		{

			return true;
		}
	}

	return false;
}

//finds the row of NONTERMINAL s in the parse table pt
int findRow(string s)
{
	for (int i = 0; i < 40; i++)
	{
		if (pt[i][0] == s)
		{
			return i;
		}
	}
	return -1;
}

// finds the column of TERMINAL s in the parse table
int findColumn(string s)
{
	for (int i = 0; i < 42; i++)
	{
		if (pt[0][i] == s)
		{
			return i;
		}
	}
	return -1;
}

//separate grammar when a blank space is detected
void removeBlank(string s)
{
	string word = "";
	string last = "";
	for (auto x : s)
	{
		if (x == ' ')
		{
			tempStack.push(word);
			last = word;
			word = "";
		}
		else
		{
			word = word + x;
		}
	}
	if (word != last)
	{
		tempStack.push(word);
	}

}


//The actual parser function
string parser(ToLexCont toLex)
{
	string input = toLex.lexeme;

	//add current token/lexeme pair to output file
	output.push_back("\n\nToken:\t" + toLex.token + "     Lexeme:\t" + toLex.lexeme);

	if (toLex.token == "IDENTIFIER" || toLex.token == "REAL" || toLex.token == "DECIMAL")
	{
		input = toLex.token;
	}

	//optional start - i chose to ignore %%
	if (input == "%%")
	{
		myStack.pop();
		return "";
	}

	//if the stack is completely empty, push $
	if (myStack.size() == 0)
	{
		myStack.push("$");
	}

	//if the stack is size 1, push <Start>
	if (myStack.size() == 1)
	{
		string start = "<Start>";
		myStack.push(start);
	}

	bool found = false;

	//loop while production rule has not been found
	while (!found)
	{
		string currentTop = myStack.top();

		//epsilon, pop stack
		if (currentTop == "<Empty>")
		{
			output.push_back("\t <Empty> -> Epsilon");
			myStack.pop();
			continue;


		}
		//current top of stack is terminal. Either it matches with input or we have an error
		if (checkIfTerminal(currentTop))
		{
			//case for matching with input
			if (currentTop == input)
			{
				found = true;
				myStack.pop();
				return "";
				break;
			}
			//does not match - error
			else
			{
				output.push_back("\n\t !!! ERROR in Line " + to_string(toLex.lineNumber) + "\n\t Expected: " + currentTop);
				return "ERROR";
				break;
			}
		}

		if (myStack.top() != "$")
		{
			//print production rule
			output.push_back("\t " + myStack.top() + "-> " + grammar[myStack.top()]);
		}

		//find next production rule
		int curr = findRow(myStack.top());
		int currCol = findColumn(input);

		//if parse table field at row = curr and column = currCol is empty, we have an error
		if (pt[curr][currCol] == "")
		{
			output.push_back("\n\t !!! ERROR in Line " + to_string(toLex.lineNumber) + "\n\t Expected: " + grammar[myStack.top()]);
			return "ERROR";

		}
		//if not empty, process new production rule
		else
		{
			removeBlank(pt[curr][currCol]);
			myStack.pop();
			while (!tempStack.empty()) {
				myStack.push(tempStack.top());
				tempStack.pop();
			}
		}

	}
}

//This method contains the parse table built based on the first/follow set in the documentation
// and a map with grammar rules (the map is just for easier printout)

void setGrammar()
{

	string start = "<Start>";

	myStack.push("$");
	myStack.push(start);

	pt[0][0] = "";

	//NONTERMINALS
	pt[1][0] = "<Start>";
	pt[2][0] = "<OptFunctionDefinitions>";
	pt[3][0] = "<FunctionDefinitions>";
	pt[4][0] = "<FunctionDefinitionsPrime>";
	pt[5][0] = "<Function>";
	pt[6][0] = "<OptParameterList>";
	pt[7][0] = "<ParameterList>";
	pt[8][0] = "<ParameterListPrime>";
	pt[9][0] = "<Parameter>";
	pt[10][0] = "<Qualifier>";
	pt[11][0] = "<Body>";
	pt[12][0] = "<OptDeclarationList>";
	pt[13][0] = "<DeclarationList>";
	pt[14][0] = "<DeclarationListPrime>";
	pt[15][0] = "<Declaration>";
	pt[16][0] = "<IDs>";
	pt[17][0] = "<IDsPrime>";
	pt[18][0] = "<StatementList>";
	pt[19][0] = "<StatementListPrime>";
	pt[20][0] = "<Statement>";
	pt[21][0] = "<Compound>";
	pt[22][0] = "<Assign>";
	pt[23][0] = "<If>";
	pt[24][0] = "<IfPrime>";
	pt[25][0] = "<For>";
	pt[26][0] = "<Output>";
	pt[27][0] = "<Input>";
	pt[28][0] = "<While>";
	pt[29][0] = "<Condition>";
	pt[30][0] = "<Relop>";
	pt[31][0] = "<Expression>";
	pt[32][0] = "<ExpressionPrime>";
	pt[33][0] = "<Term>";
	pt[34][0] = "<TermPrime>";
	pt[35][0] = "<Factor>";
	pt[36][0] = "<Primary>";
	pt[37][0] = "<PrimaryPrime>";
	pt[38][0] = "<Do>";
	pt[39][0] = "<Empty>";

	//TERMINALS
	pt[0][1] = "IDENTIFIER";
	pt[0][2] = "REAL";
	pt[0][3] = "DECIMAL";
	pt[0][4] = "for";
	pt[0][5] = "forend";
	pt[0][6] = "if";
	pt[0][7] = "else";
	pt[0][8] = "then";
	pt[0][9] = "endif";
	pt[0][10] = "do";
	pt[0][11] = "doend";
	pt[0][12] = ":";
	pt[0][13] = ";";
	pt[0][14] = ",";
	pt[0][15] = "+";
	pt[0][16] = "-";
	pt[0][17] = "*";
	pt[0][18] = "/";
	pt[0][19] = "=";
	pt[0][20] = "and";
	pt[0][21] = "or";
	pt[0][22] = "input";
	pt[0][23] = "(";
	pt[0][24] = ")";
	pt[0][25] = "[";
	pt[0][26] = "]";
	pt[0][27] = "{";
	pt[0][28] = "}";
	pt[0][29] = "$";
	pt[0][30] = "output";
	pt[0][31] = "/=";
	pt[0][32] = ">";
	pt[0][33] = "<";
	pt[0][34] = "true";
	pt[0][35] = "false";
	pt[0][36] = "function";
	pt[0][37] = "int";
	pt[0][38] = "bool";
	pt[0][39] = "float";
	pt[0][40] = "while";
	pt[0][41] = "whileend";

	//Productions for Start
	//function
	pt[1][36] = "<OptFunctionDefinitions> <OptDeclarationList> <StatementList>";
	//int
	pt[1][37] = "<OptFunctionDefinitions> <OptDeclarationList> <StatementList>";
	//bool
	pt[1][38] = "<OptFunctionDefinitions> <OptDeclarationList> <StatementList>";
	//float
	pt[1][39] = "<OptFunctionDefinitions> <OptDeclarationList> <StatementList>";
	//{
	pt[1][27] = "<OptFunctionDefinitions> <OptDeclarationList> <StatementList>";
	//IDENTIFIER
	pt[1][1] = "<OptFunctionDefinitions> <OptDeclarationList> <StatementList>";
	//if
	pt[1][6] = "<OptFunctionDefinitions> <OptDeclarationList> <StatementList>";
	//input
	pt[1][22] = "<OptFunctionDefinitions> <OptDeclarationList> <StatementList>";
	//output
	pt[1][30] = "<OptFunctionDefinitions> <OptDeclarationList> <StatementList>";
	//while
	pt[1][40] = "<OptFunctionDefinitions> <OptDeclarationList> <StatementList>";
	//for
	pt[1][4] = "<OptFunctionDefinitions> <OptDeclarationList> <StatementList>";

	//Productions for OptFunctionDefinitions
	//function
	pt[2][36] = "<FunctionDefinitions>";

	//Epsilon

	//int
	pt[2][37] = "<Empty>";
	//bool
	pt[2][38] = "<Empty>";
	//float
	pt[2][39] = "<Empty>";
	//{
	pt[2][27] = "<Empty>";
	//IDENTIFIER
	pt[2][1] = "<Empty>";
	//if
	pt[2][6] = "<Empty>";
	//input
	pt[2][22] = "<Empty>";
	//output
	pt[2][30] = "<Empty>";
	//while
	pt[2][40] = "<Empty>";
	//for
	pt[2][4] = "<Empty>";

	//Productions for FunctionDefinitions
	pt[3][36] = "<Function> <FunctionDefinitionsPrime>";

	//Productions for FunctionDefinitionsPrime
	pt[4][36] = "<FunctionDefinitions>";

	//Epsilon

	//int
	pt[4][37] = "<Empty>";
	//bool
	pt[4][38] = "<Empty>";
	//float
	pt[4][39] = "<Empty>";
	//{
	pt[4][27] = "<Empty>";
	//IDENTIFIER
	pt[4][1] = "<Empty>";
	//if
	pt[4][6] = "<Empty>";
	//input
	pt[4][22] = "<Empty>";
	//output
	pt[4][30] = "<Empty>";
	//while
	pt[4][40] = "<Empty>";
	//for
	pt[4][4] = "<Empty>";

	//Productions for Function
	pt[5][36] = "function IDENTIFIER ( <OptParameterList> ) <OptDeclarationList> <Body>";

	//Productions for OptParameterList
	pt[6][1] = "<ParameterList>";

	//Epsilon
	//,
	pt[6][14] = "<Empty>";
	// )
	pt[6][24] = "<Empty>";

	//Productions for ParameterList
	pt[7][1] = "<Parameter> <ParameterListPrime>";


	//Productions for ParameterListPrime
	pt[8][14] = ", <ParameterList>";
	//Epsilon
	//)
	pt[8][24] = "<Empty>";

	//Productions for Parameter
	//IDENTIFIER
	pt[9][1] = "<IDs> : <Qualifier>";

	//Productions for Qualifier
	//int
	pt[10][37] = "int";
	//bool
	pt[10][38] = "bool";
	//float
	pt[10][39] = "float";

	//Productions for Body
	//{
	pt[11][27] = "{ <StatementList> }";

	//Productions for OptDeclarationList
	//int
	pt[12][37] = "<DeclarationList>";
	//bool
	pt[12][38] = "<DeclarationList>";
	//float
	pt[12][39] = "<DeclarationList>";
	//Epsilon
	//{
	pt[12][27] = "<Empty>";
	//IDENTIFIER
	pt[12][1] = "<Empty>";
	//if
	pt[12][6] = "<Empty>";
	//input
	pt[12][22] = "<Empty>";
	//output
	pt[12][30] = "<Empty>";
	//while
	pt[12][40] = "<Empty>";
	//for
	pt[12][4] = "<Empty>";

	//Productions for DeclarationList
	//int
	pt[13][37] = "<Declaration> ; <DeclarationListPrime>";
	//bool
	pt[13][38] = "<Declaration> ; <DeclarationListPrime>";
	//float
	pt[13][39] = "<Declaration> ; <DeclarationListPrime>";

	//Productions for DeclarationListPrime
	//int
	pt[14][37] = "<DeclarationList>";
	//bool
	pt[14][38] = "<DeclarationList>";
	//float
	pt[14][39] = "<DeclarationList>";

	//Epsilon
	//{
	pt[14][27] = "<Empty>";
	//IDENTIFIER
	pt[14][1] = "<Empty>";
	//if
	pt[14][6] = "<Empty>";
	//input
	pt[14][22] = "<Empty>";
	//output
	pt[14][30] = "<Empty>";
	//while
	pt[14][40] = "<Empty>";
	//for
	pt[14][4] = "<Empty>";

	//Productions for Declaration
	//int
	pt[15][37] = "<Qualifier> <IDs>";
	//bool
	pt[15][38] = "<Qualifier> <IDs>";
	//float
	pt[15][39] = "<Qualifier> <IDs>";

	//Productions for IDs
	pt[16][1] = "IDENTIFIER <IDsPrime>";

	//Productions for IDsPrime
	pt[17][14] = ", <IDs>";
	//Epsilon
	// :
	pt[17][12] = "<Empty>";
	// ;
	pt[17][13] = "<Empty>";
	// )
	pt[17][24] = "<Empty>";
	//]
	pt[17][26] = "<Empty>";

	//Productions for Statement List
	// {
	pt[18][27] = "<Statement> <StatementListPrime>";
	// IDENTIFIER
	pt[18][1] = "<Statement> <StatementListPrime>";
	// if
	pt[18][6] = "<Statement> <StatementListPrime>";
	// input
	pt[18][22] = "<Statement> <StatementListPrime>";
	//output
	pt[18][30] = "<Statement> <StatementListPrime>";
	// while
	pt[18][40] = "<Statement> <StatementListPrime>";
	//for
	pt[18][4] = "<Statement> <StatementListPrime>";

	//Productions for Statement List Prime

	// {
	pt[19][27] = "<StatementList>";
	// IDENTIFIER
	pt[19][1] = "<StatementList>";
	// if
	pt[19][6] = "<StatementList>";
	// input
	pt[19][22] = "<StatementList>";
	//output
	pt[19][30] = "<StatementList>";
	// while
	pt[19][40] = "<StatementList>";
	//for
	pt[19][4] = "<StatementList>";

	//Epsilon
	//$
	pt[19][28] = "<Empty>";
	//{
	pt[19][29] = "<Empty>";

	//ADDED
	pt[19][13] = "<Empty>";

	//Productions for Statement
	// {
	pt[20][27] = "<Compound>";
	// IDENTIFIER
	pt[20][1] = "<Assign>";
	// if
	pt[20][6] = "<If>";
	// input
	pt[20][22] = "<Input>";
	//output
	pt[20][30] = "<Output>";
	// while
	pt[20][40] = "<While>";
	//for
	pt[20][4] = "<For>";


	//Productions for Compound:
	pt[21][27] = "{ <StatementList> }";

	//Productions for Assign:
	pt[22][1] = "IDENTIFIER = <Expression> ;";

	//Productions for if:
	pt[23][6] = "if ( <Condition> ) then <Statement> <IfPrime>";

	//Productions for If Prime
	pt[24][9] = "endif";
	pt[24][7] = "else then <Statement> endif";

	//Productions for For:
	pt[25][4] = "for ( <Assign> <Condition> ; <Assign> ) <Statement> forend";


	//Production Rules for Output
	pt[26][30] = "output ( <Expression> ) ;";

	//Production Rules for Input:
	pt[27][22] = "input ( <IDs> ) ;";

	//production Rules for While
	pt[28][40] = "while ( <Condition> ) <Do> whileend";

	//Production rules for Condition
	// -
	pt[29][16] = "<Expression> <Relop> <Expression>";
	// IDENTIFIER
	pt[29][1] = "<Expression> <Relop> <Expression>";
	//REAL
	pt[29][2] = "<Expression> <Relop> <Expression>";
	//DECIMAL
	pt[29][3] = "<Expression> <Relop> <Expression>";
	//true
	pt[29][34] = "<Expression> <Relop> <Expression>";
	//false
	pt[29][35] = "<Expression> <Relop> <Expression>";
	// (
	pt[29][23] = "<Expression> <Relop> <Expression>";

	//Production Rules for Relop
	pt[30][19] = "=";
	pt[30][20] = "and";
	pt[30][21] = "or";
	pt[30][31] = "/=";
	pt[30][32] = ">";
	pt[30][33] = "<";

	//Production Rules for Expression
	// -
	pt[31][16] = "<Term> <ExpressionPrime>";
	// IDENTIFIER
	pt[31][1] = "<Term> <ExpressionPrime>";
	//REAL
	pt[31][2] = "<Term> <ExpressionPrime>";
	//DECIMAL
	pt[31][3] = "<Term> <ExpressionPrime>";
	//true
	pt[31][34] = "<Term> <ExpressionPrime>";
	//false
	pt[31][35] = "<Term> <ExpressionPrime>";
	// (
	pt[31][23] = "<Term> <ExpressionPrime>";

	//Production Rules for Expression Prime
	pt[32][15] = "+ <Term> <ExpressionPrime>";
	pt[32][16] = "- <Term> <ExpressionPrime>";

	//epsilons
	//$
	pt[32][29] = "<Empty>";
	// }
	pt[32][28] = "<Empty>";
	// {
	pt[32][27] = "<Empty>";
	//Identifier
	pt[32][1] = "<Empty>";
	//if
	pt[32][6] = "<Empty>";
	//for
	pt[32][4] = "<Empty>";
	//output
	pt[32][30] = "<Empty>";
	//input
	pt[32][22] = "<Empty>";
	//while
	pt[32][40] = "<Empty>";
	//endif
	pt[32][9] = "<Empty>";
	//else
	pt[32][7] = "<Empty>";
	// )
	pt[32][24] = "<Empty>";
	// =
	pt[32][19] = "<Empty>";
	// /=
	pt[32][31] = "<Empty>";
	//and
	pt[32][20] = "<Empty>";
	//or
	pt[32][21] = "<Empty>";
	// >
	pt[32][32] = "<Empty>";
	// <
	pt[32][33] = "<Empty>";
	// ;
	pt[32][13] = "<Empty>";


	//Production Rules for Term
	// -
	pt[33][16] = "<Factor> <TermPrime>";
	//IDENTIFIER
	pt[33][1] = "<Factor> <TermPrime>";
	//REAL
	pt[33][2] = "<Factor> <TermPrime>";
	//DECIMAL
	pt[33][3] = "<Factor> <TermPrime>";
	//true
	pt[33][34] = "<Factor> <TermPrime>";
	//false
	pt[33][35] = "<Factor> <TermPrime>";
	// (
	pt[33][23] = "<Factor> <TermPrime>";

	//Production Rules for Term Prime
	// *
	pt[34][17] = "* <Factor> <TermPrime>";
	// /
	pt[34][18] = "/ <Factor> <TermPrime>";

	//epsilons
	//+
	pt[34][15] = "<Empty>";
	// -
	pt[34][16] = "<Empty>";
	//$
	pt[34][29] = "<Empty>";
	// }
	pt[34][28] = "<Empty>";
	// {
	pt[34][27] = "<Empty>";
	//Identifier
	pt[34][1] = "<Empty>";
	//if
	pt[34][6] = "<Empty>";
	//for
	pt[34][4] = "<Empty>";
	//output
	pt[34][30] = "<Empty>";
	//input
	pt[34][22] = "<Empty>";
	//while
	pt[34][40] = "<Empty>";
	//endif
	pt[34][9] = "<Empty>";
	//else
	pt[34][7] = "<Empty>";
	// )
	pt[34][24] = "<Empty>";
	// =
	pt[34][19] = "<Empty>";
	// /=
	pt[34][31] = "<Empty>";
	//and
	pt[34][20] = "<Empty>";
	//or
	pt[34][21] = "<Empty>";
	// >
	pt[34][32] = "<Empty>";
	// <
	pt[34][33] = "<Empty>";
	// ;
	pt[34][13] = "<Empty>";

	//Production Rules for Factor
	//-
	pt[35][16] = "- <Primary>";
	//IDENTIFIER
	pt[35][1] = "<Primary>";
	//REAL
	pt[35][2] = "<Primary>";
	//DECIMAL
	pt[35][3] = "<Primary>";
	//true
	pt[35][34] = "<Primary>";
	//false
	pt[35][35] = "<Primary>";
	// ;
	pt[35][23] = "<Primary>";

	//Production Rules for Primary 
	pt[36][1] = "IDENTIFIER";
	pt[36][2] = "REAL";
	pt[36][3] = "DECIMAL";
	pt[36][34] = "true";
	pt[36][35] = "false";
	pt[36][23] = "( <Expression> )";

	//Production Rules for Primary Prime
	//[
	pt[37][25] = "[ <IDs> ]";

	//epsilons
	// *
	pt[37][17] = "<Empty>";
	// /
	pt[37][18] = "<Empty>";
	//+
	pt[37][15] = "<Empty>";
	// -
	pt[37][16] = "<Empty>";
	//$
	pt[37][29] = "<Empty>";
	// }
	pt[37][28] = "<Empty>";
	// {
	pt[37][27] = "<Empty>";
	//Identifier
	pt[37][1] = "<Empty>";
	//if
	pt[37][6] = "<Empty>";
	//for
	pt[37][4] = "<Empty>";
	//output
	pt[37][30] = "<Empty>";
	//input
	pt[37][22] = "<Empty>";
	//while
	pt[37][40] = "<Empty>";
	//endif
	pt[37][9] = "<Empty>";
	//else
	pt[37][7] = "<Empty>";
	// )
	pt[37][24] = "<Empty>";
	// =
	pt[37][19] = "<Empty>";
	// /=
	pt[37][31] = "<Empty>";
	//and
	pt[37][20] = "<Empty>";
	//or
	pt[37][21] = "<Empty>";
	// >
	pt[37][32] = "<Empty>";
	// <
	pt[37][33] = "<Empty>";
	// ;
	pt[37][13] = "<Empty>";


	//Production Rules for Do
	pt[38][10] = "do <Statement> doend";

	//epsilons
	pt[39][17] = "<Empty>";
	pt[39][18] = "<Empty>";
	pt[39][15] = "<Empty>";
	pt[39][16] = "<Empty>";
	pt[39][29] = "<Empty>";
	pt[39][28] = "<Empty>";
	pt[39][27] = "<Empty>";
	pt[39][1] = "<Empty>";
	pt[39][6] = "<Empty>";
	pt[39][8] = "<Empty>";
	pt[39][9] = "<Empty>";
	pt[39][10] = "<Empty>";
	pt[39][11] = "<Empty>";
	pt[39][30] = "<Empty>";
	pt[39][7] = "<Empty>";
	pt[39][24] = "<Empty>";
	pt[39][19] = "<Empty>";
	pt[39][31] = "<Empty>";
	pt[39][20] = "<Empty>";
	pt[39][21] = "<Empty>";
	pt[39][32] = "<Empty>";
	pt[39][33] = "<Empty>";
	pt[39][13] = "<Empty>";





	grammar[""] = "";
	grammar[""] = "";
	grammar["<Start>"] = "<OptFunctionDefinitions><OptDeclarationList><StatementList>";
	grammar["<OptFunctionDefinitions>"] = "<Function Definitions>|<Empty>";
	grammar["<FunctionDefinitions>"] = "<Function><FunctionDefinitionsPrime>";
	grammar["<FunctionDefinitionsPrime>"] = "<FunctionDefinitions>|<Empty>";
	grammar["<Function>"] = "function <Identifier > (<Opt Parameter List>) < Opt Declaration List > <Body>";
	grammar["<OptParameterList>"] = "<Parameter List>|<Empty>";
	grammar["<ParameterList>"] = "<Parameter><ParameterListPrime>";
	grammar["<ParameterListPrime>"] = ",<ParameterList>|<Empty>";
	grammar["<Parameter>"] = "<IDs>:<Qualifier>";
	grammar["<Qualifier>"] = "int, bool, float";
	grammar["<Body>"] = "{  < Statement List>  }";
	grammar["<OptDeclarationList>"] = "<Declaration List>|<Empty>";
	grammar["<DeclarationList>"] = "<Declaration> ; <DeclarationListPrime>";
	grammar["<DeclarationListPrime>"] = "<Declaration List> | <Empty>";
	grammar["<Declaration>"] = "<Qualifier > <IDs>";
	grammar["<IDs>"] = "IDENTIFIER|IDENTIFIER, <IDs>";
	grammar["<IDsPrime>"] = ", <IDs> | <Empty>";
	grammar["<StatementList>"] = "<Statement> <Statement List Prime>";
	grammar["<StatementListPrime>"] = "<Statement List>|<Empty>";
	grammar["<Statement>"] = "<Compound>|<Assign>|<If>|<For>|<While>|<Input>|<Output>";
	grammar["<Compound>"] = "{  <Statement List>  }";
	grammar["<Assign>"] = "<Identifier>=<Expression>;";
	grammar["<If>"] = "if  ( <Condition>  ) then <Statement> <If Prime>";
	grammar["<IfPrime>"] = "endif |else then <Statement> endif";
	grammar["<For>"] = "for(<Assign> <Condition>; <Assign>) <Statement> forend";
	grammar["<Output>"] = "output ( <Expression>);";
	grammar["<Input>"] = "input ( <IDs> );";
	grammar["<Do>"] = "do <Statement> doend";
	grammar["<While>"] = "while ( <Condition>  ) <Do> whileend";
	grammar["<Condition>"] = "<Expression>  <Relop>   <Expression>";
	grammar["<Relop>"] = "=|/=|>|<|and|or";
	grammar["<Expression>"] = "<Term><ExpressionPrime>";
	grammar["<ExpressionPrime>"] = "+<Term><ExpressionPrime>|- <Term><ExpressionPrime>|epsilon";
	grammar["<Term>"] = "<Factor><TermPrime>";
	grammar["<TermPrime>"] = "*<Factor><TermPrime>|/ <Factor><TermPrime>|e";
	grammar["<Factor>"] = "-<Primary>|<Primary>";
	grammar["<Primary>"] = "IDENTIFIER|DECIMAL|IDENTIFIER[<IDs>]|(<Expression>)|REAL|true|false";
	grammar["<Empty>"] = "epsilon";
	grammar["<PrimaryPrime>"] = "[<IDs>]|<Empty>";

}

vector<string> getOutputFile()
{
	return output;
}
