/*
	Johanna Nguyen
	Gilbert Paderogo
	Richard Phan

	CPSC 323 Compilers and Languages
	Syntax Analyzer
	November 5, 2019
*/

#ifndef lexicalAnalyzer_hpp
#define lexicalAnalyzer_hpp

#include <iostream>
#include <fstream>
#include <iomanip>
#include <tuple>
#include <string>
#include <vector>
using namespace std;

//Returns column number based on type of character
int columnNum(char c);

//Identifiers
bool isIdentifier(const string& str);

//Keywords
bool isKeyword(const string& str);

//Select Keywords
bool isSelect(const string& str);

//Integers
bool isInteger(const string& str);

//Floating Point
bool isFloat(const string& str);

//Separators
bool isSeparator(const string& lex, string& nextLex, ifstream& inFile);

//Operators
bool isOperator(const string& lex, string& nextLex, ifstream& inFile);

//Comments
bool isComment(char c, ifstream& inFile);

//Does actual lexer work
tuple<string, string> backgroundLex(ifstream& inFile);

//Prints the lexer
tuple<string, string> lexer(ifstream& inFile, ofstream& outFile);

#endif