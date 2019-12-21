/*
	Johanna Nguyen
	Gilbert Paderogo
	Richard Phan

	CPSC 323 Compilers and Languages
	Syntax Analyzer
	November 5, 2019
*/

#ifndef assembly_hpp
#define assembly_hpp

/*
	Johanna Nguyen
	Gilbert Paderogo
	Richard Phan

	CPSC 323 Compilers and Languages
	Lexical Analyzer
	September 19, 2019
*/

#include <iostream>
#include <fstream>
#include <iomanip>
#include <tuple>
#include <string>
#include <vector>
using namespace std;


struct Instruction {
	int address;
	string operation;
	string operand;
};

vector<tuple<string, int, string>> genSymbols(vector<tuple<string, string>> list);

bool checkDuplicate(vector<tuple<string, int, string>> table, string id);


#endif
#pragma once