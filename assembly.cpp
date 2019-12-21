/*
	Johanna Nguyen
	Gilbert Paderogo
	Richard Phan

	CPSC 323 Compilers and Languages
	Lexical Analyzer
	September 19, 2019
*/

#include "assembly.h"

vector<tuple<string, int, string>> table;
tuple<string, int, string> item;

int memLoc = 5000;
int i = 0;

string currentKey;
string identifier;

bool repeat = false;

ofstream outFile("symbolTable.txt");

vector<tuple<string, int, string>> genSymbols(vector<tuple<string, string>> list)
{
	while (i < list.size())
	{
		if (get<0>(list.at(i)) == "KEYWORD")
		{
			currentKey = get<1>(list.at(i));
		}

		if (get<0>(list.at(i)) == "IDENTIFIER" && (checkDuplicate(table, get<1>(list.at(i))) == false))
		{
			identifier = get<1>(list.at(i));
			table.push_back(make_tuple(identifier, memLoc, currentKey));
			++memLoc;
		}
		++i;
	}

	outFile << endl << "----- SYMBOL TABLE -----" << endl;
	outFile << "Identifier\t" << "Memory Location\t\t" << "Type" << endl;
	for (auto i : table) {
		outFile << get<0>(i) << "\t\t"
			<< get<1>(i) << "\t\t\t"
			<< get<2>(i) << endl;
	}
	
	return table;
}

bool checkDuplicate(vector<tuple<string, int, string>> table, string id)
{
	int i = 0;
	while (i < table.size())
	{
		if (get<0>(table.at(i)) == id)
		{
			return true;
		}
		++i;
	}
	return false;
}
