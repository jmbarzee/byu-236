#include <string>
#include <fstream>
#include <iostream>

#include "Token.h"
#include "LexicalAnalyzer.h"
#include "Parser.h"

using namespace std;

int main(int argc, const char* argv[]) {
	ofstream outfile;
	outfile.open(argv[2]);
	LexicalAnalyzer lexigraph(argv[1]);
	vector<Token> tokens = lexigraph.getAllTokens();
	/*bool brokenToken = false;
	for (int i = 0; i < tokens.size(); i++) {
		if (tokens[i].getName() == "UNDEFINED") {
			brokenToken = true;
			outfile << "Input Error on line " << tokens[i].getLine() << endl;
			break; //remove to see all errors upon parsing
		} else {
			outfile << tokens[i].toString();
		}
	}
	if (!brokenToken) {
		outfile << "Total Tokens = " << tokens.size() << endl;
	}*/
	Parser parser = Parser(tokens);
	 bool success = parser.parse();
	 if (!success) {
	 outfile << "Failure!" << endl;
	 outfile << "  " << parser.getLastToken().toString();
	 } else {
	 outfile << "Success!" << endl;
	 outfile << parser.getDataLog().toString();
	 }
}
