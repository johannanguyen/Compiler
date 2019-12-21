/*
	Johanna Nguyen
	Gilbert Paderogo
	Richard Phan
	CPSC 323 Compilers and Languages
	Syntax Analyzer
	November 5, 2019
*/

#include "lexicalAnalyzer.h"


//Returns column number based on character type
int columnNum(char c)
{
	if (isalpha(c))			return 0;			//column 0 = letter
	else if (isdigit(c))	return 1;			//column 1 = digit
	else if (c == '$')		return 2;			//column 2 = $
	else if (c == '.')		return 3;			//column 3 = .
	return 4;									//column 4 = all other characters
}


//Returns true if identifier
bool isIdentifier(const string& str)
{												//DFSM TABLE
	const int STATES = 4, INPUTS = 5;			//		l	d	$	.	other
	int dfsmTable[STATES][INPUTS] =				//0	|	0	0	0	0	0	(dead state)
	{ { 0, 0, 0, 0, 0 },						//1	|	2	0	0	0	0
	{ 2, 0, 0, 0, 0 },							//2	|	2	2	3	0	0
	{ 2, 2, 3, 0, 0 },							//3	|	0	0	0	0	0
	{ 0, 0, 0, 0, 0 } };						//Final states: 2, 3

	int state = 1;								//Initial state: 1
	int length = str.length();
	for (int i = 0; i < length; ++i)			//Traverses through lexeme
	{
		int input = columnNum(str[i]);			//Finds column number based on each character in the lexeme
		state = dfsmTable[state][input];		//Determines new state based on input
	}
	if (state == 2 || state == 3)
		return true;

	return false;
}


//Returns true if keyword
bool isKeyword(const string& lex)
{
	string keyword[11] = { "function", "int", "boolean", "real",
		 "return", "put", "get", "true", "false", "whileend", "endif" };

	size_t lexLength = lex.length();
	for (int i = 0; i <= 13; ++i)				//Directly compares lexeme to already-known keywords
	{
		if (lex == keyword[i])
			return true;
	}
	return false;
}


//Returns if select keyword
bool isSelect(const string& lex)
{
	string keyword[5] = { "while", "for", "if", "else", "whileend" };

	size_t lexLength = lex.length();
	for (int i = 0; i <= 13; ++i)				//Directly compares lexeme to already-known keywords
	{
		if (lex == keyword[i])
			return true;
	}
	return false;
}


//Returns true if integer
bool isInteger(const string& str)
{												//DFSM TABLE
	const int STATES = 3, INPUTS = 5;			//		l	d	$	.	other
	int dfsmTable[STATES][INPUTS] =				//0	|	0	0	0	0	0	(dead state)
	{ { 0, 0, 0, 0, 0 },						//1	|	0	2	0	0	0
	{ 0, 2, 0, 0, 0 },							//2	|	0	2	0	0	0
	{ 0, 2, 0, 0, 0 } };						//Final state: 2

	int state = 1;								//Initial state: 1
	int length = str.length();
	for (int i = 0; i < length; ++i)			//Traverses through lexeme
	{
		int input = columnNum(str[i]);			//Finds column number based on each character in the lexeme
		state = dfsmTable[state][input];		//Determines new state based on input
	}
	if (state == 2)
		return true;

	return false;
}


//Returns true if float
bool isFloat(const string& str)
{
	const int STATES = 5, INPUTS = 5;			//		l	d	$	.	other
	int dfsmTable[STATES][INPUTS] =				//0	|	0	0	0	0	0	(dead state)
	{ { 0, 0, 0, 0, 0 },						//1	|	0	2	0	3	0
	{ 0, 2, 0, 3, 0 },							//2	|	0	2	0	3	0
	{ 0, 2, 0, 3, 0 },							//3	|	0	4	0	0	0
	{ 0, 4, 0, 0, 0 },							//4	|	0	4	0	0	0
	{ 0, 4, 0, 0, 0 } };						//Final state: 4

	int state = 1;								//Initial state: 1
	int length = str.length();
	for (int i = 0; i < length; ++i)			//Traverses through lexeme
	{
		int input = columnNum(str[i]);			//Finds column number based on each character in the lexeme
		state = dfsmTable[state][input];		//Determines new state based on input
	}
	if (state == 4)
		return true;

	return false;
}


//Returns true if separator
bool isSeparator(const string& lex, string& nextLex, ifstream& inFile)
{
	string separators[7] = { "(", ")", ",", "{", "}", ";", ":" };

	for (int i = 0; i < 7; ++i)
	{
		if (nextLex == separators[i])				//Directly compares lexeme to already-known separators
			return true;
	}
	return false;
}


//Returns true if operator
bool isOperator(const string& lex, string& nextLex, ifstream& inFile)
{
	string operators[7] = { "+", ">", "<", "-", "*", "/", "=" };
	for (int i = 0; i < 7; ++i)
	{
		if (operators[i] == nextLex)				//Directly compares lexeme to already-known operators
			return true;
	}
	return false;
}


//Return true if comment
bool isComment(char c, ifstream& inFile)
{
	char next;
	if (c == '!')									//Identifies beginning of comment
	{
		inFile.get(next);							//Processes next character to see if it's another !
		char current;
		bool endComment = false;

		if (next == '!')
			endComment = true;

		while (inFile && !endComment)				//If next character is not !
		{
			current = next;
			inFile.get(next);						//Keeps processing

			if (next == '!')						//Until it finds end of comment
				endComment = true;
		}
	}
	return false;
}


//Does actual lexer work
tuple<string, string> backgroundLex(ifstream& inFile)
{
	char c;
	string lex, nextLex;
	bool endLex = false;

	if (inFile.is_open())
	{
		inFile.get(c);											//Reads in first character

		while (inFile && (c == ' '))				//Reads leading whitespace
		{
			inFile.get(c);
		}
		if (!inFile.eof())
			inFile.putback(c);

		while (!inFile.eof() && !endLex)						//Starts to build lexeme until a separator or operator is met
		{
			inFile.get(c);

			nextLex = c;

			if (c == '\n')
			{
				return make_tuple("EOS", "$");
			}

			else if (isspace(c))
			{
				if (lex != "")									//In case a comment was found at end of lexeme
					endLex = true;
			}

			else if (c == '!')
				isComment(c, inFile);

			else if (isSeparator(lex, nextLex, inFile))
			{
				if (lex == "")									//Start of lexeme is found
					return make_tuple("SEP", nextLex);

				inFile.putback(c);								//Separator was found, put back
				endLex = true;
			}

			else if (isOperator(lex, nextLex, inFile))
			{
				if (lex == "")									//Start of lexeme is found
					return make_tuple("OPERATOR", nextLex);

				inFile.putback(c);								//Operator was found, put back
				endLex = true;
			}
			else
				lex += c;										//End of lexeme found
		}

		if (isIdentifier(lex))									//Checks if lexeme is identifier or keyword
		{
			if (isKeyword(lex))
				return make_tuple("KEYWORD", lex);

			else if (isSelect(lex))
				return make_tuple("SELECT", lex);

			else
				return make_tuple("IDENTIFIER", lex);
		}
		else if (isInteger(lex))
			return make_tuple("INTEGER", lex);					//Checks if lexeme is integer

		else if (isFloat(lex))
			return make_tuple("FLOAT", lex);					//Checks if lexeme is floating point number

		else
			return make_tuple("END OF FILE", lex);				//If lexeme could not be categorized into a token
	}
	else
	{
		cerr << "Error - could not open file.";
		return make_tuple("UNKNOWN", lex);
	}
}


//Provides each token/lexeme pair with proper formatting
tuple<string, string> lexer(ifstream& inFile, ofstream& outFile)
{

	tuple<string, string> token = backgroundLex(inFile);

	outFile << endl << left << setw(20) << get<0>(token)
		<< setw(20) << "="
		<< setw(20) << get<1>(token);

	return token;
}