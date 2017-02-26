#include <string>
#include <fstream>
#include <vector>
#include <string>

#include "Token.h"

using namespace std;

class LexicalAnalyzer {
private:
	int crnt;
	ifstream in;
	int currentLine;
	int beginingLine;
	vector<Token> tokens;
public:
	LexicalAnalyzer(string inName) {
		in.open(inName.c_str());
		currentLine = 1;
		beginingLine = currentLine;
		crnt = ' ';
	}
	~LexicalAnalyzer() {
		this->in.close();
	}
	Token getToken();
	vector <Token> getAllTokens();
	void whiteSpace();
	void colonDash();
	void comment();
	void strings();
	void clearID();
	void keyword(string id);
};
