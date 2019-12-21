/*
	Johanna Nguyen
	Gilbert Paderogo
	Richard Phan
	CPSC 323 Compilers and Languages
	Syntax Analyzer
	November 5, 2019
*/

#include "syntaxAnalyzer.h"
#include "lexicalAnalyzer.h"
#include "assembly.h"
using namespace std;

void genInstruction(string operation, string operand);
string getAddress(vector<tuple<string, int, string>> table, string token);
int instrAddress = 1;
vector<Instruction> instrTable;

int	analyzer(vector<tuple<string, string>> list)
{
	ofstream 			outFile("syntaxAnalyzer.txt");
	ofstream			out("assembly.txt");
	stack<string>		tableStack;
	int					iterator = 0;
	int					numStatements = 1;
	int					row;
	int					col;
	string				currentTop;
	string				currentToken;
	string				currentLexeme;

	string 				parserTable[13][25] = { {"-1",    "while",    "{",    "}",   "id",   "=",   "+",   "-",   "*",   "/",   "(",    ")",   "integer",  "bool",  "if",  "else",  ";",    ",",   "<",   ">",   "==",   "int",   "boolean",   "$",  "endif"},
												 {"S",    "3",        "24",   "25",  "1",    "-1",  "-1",  "-1",  "-1",  "-1",  "-1",   "-1",  "-1",       "-1",    "15",  "26",    "-1",   "-1",  "-1",  "-1",  "-1",   "2",     "2",         "e",  "27" },
												 {"A",    "-1",       "-1",   "-1",  "4",    "-1",  "-1",  "-1",  "-1",  "-1",  "-1",   "-1",  "-1",       "-1",    "-1",  "-1",    "-1",   "-1",  "-1",  "-1",  "-1",   "-1",    "-1",        "-1",  "-1" },
												 {"E",    "-1",       "-1",   "-1",  "5",    "-1",  "-1",  "-1",  "-1",  "-1",  "5",    "-1",  "5",        "5",     "-1",  "-1",    "-1",   "-1",  "-1",  "-1",  "-1",   "-1",    "-1",        "-1",  "-1" },
												 {"E'",   "-1",       "e",    "e",   "-1",   "-1",  "6",   "7",   "-1",  "-1",  "-1",   "e",   "-1",       "-1",    "-1",  "-1",    "e",    "-1",  "e",   "e",   "e",    "-1",    "-1",        "e",   "-1" },
												 {"T",    "-1",       "-1",   "-1",  "8",    "-1",  "-1",  "-1",  "-1",  "-1",  "8",    "-1",  "8",        "8",     "-1",  "-1",    "-1",   "-1",  "-1",  "-1",  "-1",   "-1",    "-1",        "-1",  "-1" },
												 {"T'",   "-1",       "e",    "e",   "1",    "-1",  "e",   "e",   "9",   "10",  "-1",   "e",   "-1",       "-1",    "-1",   "-1",   "e",    "-1",  "e",   "e",   "e",    "-1",    "-1",        "e",   "-1" },
												 {"F",    "-1",       "-1",   "-1",  "12",   "-1",  "-1",  "-1",  "-1",  "-1",  "11",   "-1",  "13",       "14",    "-1",   "-1",   "-1",   "-1",  "-1",  "-1",  "-1",   "-1",    "-1",        "-1",  "-1" },
												 {"D",    "-1",       "-1",   "e",   "1",    "-1",  "-1",  "-1",  "-1",  "-1",  "-1",   "-1",  "-1",       "-1",    "-1",   "-1",   "-1",   "-1",  "-1",  "-1",  "-1",   "16",    "16",        "e",   "-1" },
												 {"C",    "-1",       "-1",   "-1",  "17",   "-1",  "-1",  "-1",  "-1",  "-1",  "17",   "-1",  "17",       "17",    "-1",   "-1",   "-1",   "-1",  "-1",  "-1",  "-1",   "-1",    "-1",        "-1",  "-1" },
												 {"R",    "-1",       "-1",   "-1",  "-1",   "-1",  "-1",  "-1",  "-1",  "-1",  "-1",   "-1",  "-1",       "-1",    "-1",   "-1",   "-1",   "-2",  "18",  "19",  "20",   "-1",    "-1",        "-1",  "-1" },
												 {"TYPE", "-1",       "-1",   "-1",  "1",    "-1",  "-1",  "-1",  "-1",  "-1",  "-1",   "-1",  "-1",       "-1",    "-1",   "-1",   "-1",   "-1",  "-1",  "-1",  "-1",   "21",    "22",        "-1",  "-1" },
												 {"M",    "-1",       "-1",   "-1",  "-1",   "-1",  "-1",  "-1",  "-1",  "-1",  "-1",   "-1",  "-1",       "-1",    "-1",   "-1",   "e",    "23",  "-1",  "-1",  "-1",   "-1",    "-1",        "-1",  "-1" },
	};

	for (int i = 0; i < list.size(); i++) {
		if (get<0>(list.at(i)) == "EOS") {
			numStatements++;
		}
	}

	vector<tuple<string, int, string>> symbolTable = genSymbols(list);

	for (int i = 0; i < numStatements; i++) {
		// push "$" and starting "S" symbol
		tableStack.push("$");
		tableStack.push("S");

		// add "$" to the end of stringCheck 
		list.at(list.size() - 1) = make_tuple("EOS", "$");


		while (!tableStack.empty())
		{
			currentTop = tableStack.top();
			currentToken = get<0>(list.at(iterator));
			currentLexeme = get<1>(list.at(iterator));

			if (currentToken == "INTEGER")
			{
				currentLexeme = "integer";
			}

			if (currentToken == "KEYWORD")
			{
				if (currentLexeme == "true" || currentLexeme == "false")
				{
					currentLexeme = "bool";
				}
			}

			if (currentToken == "SELECT")
			{
				currentToken = "KEYWORD";
			}

			if (currentToken == "IDENTIFIER")
			{
				currentToken = "id";
				currentLexeme = "id";
			}

			// terminal at top of stack
			if (isTerminal(currentTop))
			{
				if (currentTop == currentLexeme)
				{
					outFile << "Token: " << currentToken << "\t\tLexeme: " << get<1>(list.at(iterator)) << endl << endl;
					tableStack.pop();
					++iterator;
				}

				// error message
				else
				{
					outFile << "Error...unexpected terminal..." << endl;
					outFile << "Current terminal is: " << currentLexeme << endl;
					outFile << "Expected terminal is: " << currentTop << endl;
					outFile << "Exiting parser..." << endl << endl;
					outFile.close();

					cout << "Error occured please check 'output.txt for details" << endl;
					exit(EXIT_FAILURE);

				}
			}

			// non-terminal, so check which rule to use and push to stack 
			else
			{
				row = getRow(currentTop);
				col = getCol(currentLexeme);

				if (row == -1) {
					outFile << "Error: unexpected nonterminal" << endl;
				}
				if (col == -1) {
					outFile << "Error: unexpected terminal" << endl;
				}

				tableStack.pop();
				printRule(parserTable[row][col], outFile);

				if (parserTable[row][col] == "1")
				{
					tableStack.push("A");
				}
				else if (parserTable[row][col] == "2")
				{
					tableStack.push("D");
				}
				else if (parserTable[row][col] == "3")
				{
					tableStack.push(")");
					tableStack.push("C");
					tableStack.push("(");
					tableStack.push("while");

				}
				else if (parserTable[row][col] == "4")
				{
					tableStack.push(";");
					tableStack.push("E");
					tableStack.push("=");
					tableStack.push("id");
					genInstruction("PUSHI", "intvalue");
					genInstruction("POPM", getAddress(symbolTable, get<1>(list.at(iterator))));
				}
				else if (parserTable[row][col] == "5")
				{
					tableStack.push("E'");
					tableStack.push("T");
				}
				else if (parserTable[row][col] == "6")
				{
					tableStack.push("E'");
					tableStack.push("T");
					tableStack.push("+");
					genInstruction("ADD", "nil");
				}
				else if (parserTable[row][col] == "7")
				{
					tableStack.push("E'");
					tableStack.push("T");
					tableStack.push("-");
					genInstruction("SUB", "nil");
				}
				else if (parserTable[row][col] == "8")
				{
					tableStack.push("T'");
					tableStack.push("F");
				}
				else if (parserTable[row][col] == "9")
				{
					tableStack.push("T'");
					tableStack.push("F");
					tableStack.push("*");
					genInstruction("MUL", "nil");
				}
				else if (parserTable[row][col] == "10")
				{
					tableStack.push("T'");
					tableStack.push("F");
					tableStack.push("/");
					genInstruction("DIV", "nil");
				}

				else if (parserTable[row][col] == "11")
				{
					tableStack.push(")");
					tableStack.push("E");
					tableStack.push("(");
				}
				else if (parserTable[row][col] == "12")
				{
					tableStack.push("id");
					genInstruction("PUSHM", getAddress(symbolTable, get<1>(list.at(iterator))));
				}
				else if (parserTable[row][col] == "13")
				{
					tableStack.push("integer");
				}
				else if (parserTable[row][col] == "14")
				{
					tableStack.push("bool");
				}
				else if (parserTable[row][col] == "15")
				{
					tableStack.push(")");
					tableStack.push("C");
					tableStack.push("(");
					tableStack.push("if");
				}
				else if (parserTable[row][col] == "16")
				{
					tableStack.push(";");
					tableStack.push("M");
					tableStack.push("id");
					tableStack.push("TYPE");
					genInstruction("PUSHI", "intvalue");
					genInstruction("POPM", getAddress(symbolTable, get<1>(list.at(iterator))));
				}
				else if (parserTable[row][col] == "17")
				{
					tableStack.push("E");
					tableStack.push("R");
					tableStack.push("E");
				}
				else if (parserTable[row][col] == "18")
				{
					tableStack.push("<");
				}
				else if (parserTable[row][col] == "19")
				{
					tableStack.push(">");
				}
				else if (parserTable[row][col] == "20")
				{
					tableStack.push("==");
				}
				else if (parserTable[row][col] == "21")
				{
					tableStack.push("int");
				}
				else if (parserTable[row][col] == "22")
				{
					tableStack.push("boolean");
				}
				else if (parserTable[row][col] == "23")
				{
					tableStack.push("M");
					tableStack.push("id");
					tableStack.push(",");
					//genInstruction("PUSHI", "intvalue");
					//genInstruction("POPM", getAddress(symbolTable, get<1>(list.at(iterator))));
				}
				else if (parserTable[row][col] == "24")
				{
					tableStack.push("{");
				}
				else if (parserTable[row][col] == "25")
				{
					tableStack.push("}");
				}
				else if (parserTable[row][col] == "26")
				{
					tableStack.push("else");
				}
				else if (parserTable[row][col] == "27")
				{
					tableStack.push("endif");
				}
			}
		}
		outFile << "\n---------- END OF STATEMENT ----------\n" << endl;
	}
	outFile.close();

	out << "Address\t" << "Op\t" << "Operand" << endl;
	for (auto instr : instrTable) {
		out << instr.address << "\t" << instr.operation << "\t" << instr.operand << endl;
	}
	return 0;
}

//Displays which production rule is used
void printRule(string ruleNum, ofstream& output)
{
	if (ruleNum == "1")
	{
		//S -> A
		output << "<Statement>\t->\t<Assignment>" << endl;
	}
	else if (ruleNum == "2")
	{
		//S -> D
		output << "<Statement>\t->\t<Declarative>" << endl;
	}
	else if (ruleNum == "3")
	{
		//S -> while ( C )
		output << "<Statement>\t->\twhile ( <Conditional> )" << endl;
	}
	else if (ruleNum == "4")
	{
		//A -> id = E;
		output << "<Assignment>\t->\tid = <Expression>;" << endl;
	}
	else if (ruleNum == "5")
	{
		//E -> TE'
		output << "<Expression>\t->\t<Term> <ExpressionPrime>" << endl;
	}
	else if (ruleNum == "6")
	{
		//E' -> +TE
		output << "<ExpressionPrime>\t\t->\t+ <Term> <ExpressionPrime>" << endl;
	}
	else if (ruleNum == "7")
	{
		//E' -> -TE'
		output << "<ExpressionPrime>\t\t->\t- <Term> <ExpressionPrime>" << endl;
	}
	else if (ruleNum == "8")
	{
		//T -> FT'
		output << "<Term>\t\t->\t<Factor> <TermPrime>" << endl;
	}
	else if (ruleNum == "9")
	{
		//T' -> *FT
		output << "<TermPrime>\t->\t* <Factor> <TermPrime>" << endl;
	}
	else if (ruleNum == "10")
	{
		//T' -> /FT'
		output << "<TermPrime>\t->\t/ <Factor> <TermPrime>" << endl;
	}
	else if (ruleNum == "11")
	{
		//F -> ( E )
		output << "<Factor>\t->\t( <Expression> )" << endl;
	}
	else if (ruleNum == "12")
	{
		//F -> id
		output << "<Factor>\t->\tid" << endl;
	}
	else if (ruleNum == "13")
	{
		//F -> integer
		output << "<Factor>\t->\tinteger" << endl;
	}
	else if (ruleNum == "14")
	{
		//F -> bool
		output << "<Factor>\t->\tbool" << endl;
	}
	else if (ruleNum == "15")
	{
		//S -> if ( C )
		output << "<Statement>\t->\tif ( <Conditional> )" << endl;
	}
	else if (ruleNum == "16")
	{
		//D -> TYPE id M;
		output << "<Declarative>\t->\t<TYPE> id <MoreIds>;" << endl;
	}
	else if (ruleNum == "17")
	{
		//C -> E R E
		output << "<Conditional>\t->\t<Expression> <Relational> <Expression>" << endl;
	}
	else if (ruleNum == "18")
	{
		//R -> <
		output << "<Relational>\t->\t<" << endl;
	}
	else if (ruleNum == "19")
	{
		//R -> >
		output << "<Relational>\t->\t>" << endl;
	}
	else if (ruleNum == "20")
	{
		//R -> ==
		output << "<Relational>\t->\t==" << endl;
	}
	else if (ruleNum == "21")
	{
		//TYPE -> int
		output << "<TYPE>\t->\tint" << endl;
	}
	else if (ruleNum == "22")
	{
		//TYPE -> boolean
		output << "<TYPE>\t->\tboolean" << endl;
	}
	else if (ruleNum == "23")
	{
		//M -> , id M | e
		output << "<MoreIds>\t->\t, id <MoreIds>" << endl;
	}
	else if (ruleNum == "24")
	{
		//S -> {
		output << "<Statement>\t->\t{" << endl;
	}
	else if (ruleNum == "25")
	{
		//S -> }
		output << "<Statement>\t->\t}" << endl;
	}
	else if (ruleNum == "26")
	{
		//S -> else
		output << "<Statement>\t->\telse" << endl;
	}
	else if (ruleNum == "27")
	{
		//S -> endif
		output << "<Statement>\t->\tendif" << endl;
	}
	else if (ruleNum == "e")
	{
		//epsilon
		//Not necessary since we don't know need to point this out
		output << "";
	}

}

//Determines if string is a terminal
bool isTerminal(string check)
{
	if (check == "while" || check == "{" || check == "}" || check == "id" ||
		check == "=" || check == "+" || check == "-" || check == "*" ||
		check == "/" || check == "(" || check == ")" || check == "integer" ||
		check == "bool" || check == "if" || check == "else" || check == ";" ||
		check == "," || check == "<" || check == ">" || check == "==" ||
		check == "int" || check == "boolean" || check == "$" || check == "endif")
	{
		return true;
	}
	return false;
}

//Used in parsing table
int	getRow(string check)
{
	if (check == "S")
		return 1;
	else if (check == "A")
		return 2;
	else if (check == "E")
		return 3;
	else if (check == "E'")
		return 4;
	else if (check == "T")
		return 5;
	else if (check == "T'")
		return 6;
	else if (check == "F")
		return 7;
	else if (check == "D")
		return 8;
	else if (check == "C")
		return 9;
	else if (check == "R")
		return 10;
	else if (check == "TYPE")
		return 11;
	else if (check == "M")
		return 12;
	else
		return -1;
}

//Used in parsing table
int	getCol(string check)
{
	if (check == "while")
		return 1;
	else if (check == "{")
		return 2;
	else if (check == "}")
		return 3;
	else if (check == "id")
		return 4;
	else if (check == "=")
		return 5;
	else if (check == "+")
		return 6;
	else if (check == "-")
		return 7;
	else if (check == "*")
		return 8;
	else if (check == "/")
		return 9;
	else if (check == "(")
		return 10;
	else if (check == ")")
		return 11;
	else if (check == "integer")
		return 12;
	else if (check == "bool")
		return 13;
	else if (check == "if")
		return 14;
	else if (check == "else")
		return 15;
	else if (check == ";")
		return 16;
	else if (check == ",")
		return 17;
	else if (check == "<")
		return 18;
	else if (check == ">")
		return 19;
	else if (check == "==")
		return 20;
	else if (check == "int")
		return 21;
	else if (check == "boolean")
		return 22;
	else if (check == "$")
		return 23;
	else if (check == "endif")
		return 24;
	else
		return -1;
}

void genInstruction(string operation, string operand) {
	Instruction instr;

	instr.address = instrAddress;
	instr.operation = operation;
	instr.operand = operand;

	instrTable.push_back(instr);
	instrAddress++;
}

string getAddress(vector<tuple<string, int, string>> table, string token) {
	for (auto i : table) {
		if (get<0>(i) == token) {
			return to_string(get<1>(i));
		}
	}
}