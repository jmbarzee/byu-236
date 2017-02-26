#include <string>
#include <fstream>
#include <iostream>

#include "Token.h"
#include "LexicalAnalyzer.h"
#include "Parser.h"
#include "Database.h"

using namespace std;

int main(int argc, const char* argv[]) {
	ofstream outfile;
	outfile.open(argv[2]);
	LexicalAnalyzer lexigraph(argv[1]);
	vector<Token> tokens = lexigraph.getAllTokens();
	Parser parser = Parser(tokens);
	parser.parse();
	Database dataBase = Database(parser.getDataLog(), &outfile);
}
