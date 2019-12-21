/*
	Johanna Nguyen
	Gilbert Paderogo
	Richard Phan

	CPSC 323 Compilers and Languages
	Lexical Analyzer
	September 19, 2019
*/

#include "lexicalAnalyzer.h"
#include "syntaxAnalyzer.h"
#include "assembly.h"
#include <map>


int main()
{
	ifstream inFile;
	string userInput;
	ofstream outFile("syntaxAnalyzer.txt");
	vector<tuple<string, string>> list;

	cout << "------------------------------------------------------------\n"
		<< "COMPILER by:\n"
		<< "Johanna Nguyen, Gilbert Paderogo, Richard Phan\n"
		<< "------------------------------------------------------------\n"
		<< "Enter a text file location and name.\n"
		<< "Remember to use double slashes!\n\n"
		<< "Example: C:\\\\Users\\\\Desktop\\\\input.txt\n"
		<< "------------------------------------------------------------\n"
		<< "Your file name: ";

	cin >> userInput;				//Saves file destination as string

	inFile.open(userInput);
	if (inFile.is_open())
	{

		while (!inFile.eof())
		{
			tuple<string, string> token = backgroundLex(inFile); // puts data into vector
			list.push_back(token);
		}

		analyzer(list);

		cout << "------------------------------------------------------------\n"
			<< "File has successfully been processed.\n\n"
			<< "Your results have been printed to \"syntaxAnalyzer.txt.\"\n"
			<< "Symbol table has been printed to \"symbolTable.txt\"\n"
			<< "Assembly code has been printed to \"assembly.txt.\"\n\n";
	}
	else
		cerr << "------------------------------------------------------------\n"
		<< "\nError - file could not be opened.\n\n";

	inFile.close();
	outFile.close();

	system("Pause");
	return 0;
}
