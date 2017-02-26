/*
 * LexicalAnalyzer.cpp
 *
 *  Created on: May 6, 2015
 *      Author: jacobmb
 */
#include <string>
#include <vector>
#include <cstdlib>
#include <iostream>

#include "LexicalAnalyzer.h"
#include "Token.h"

using namespace std;
vector<Token> LexicalAnalyzer::getAllTokens() {
	while (!in.eof()) {
		getToken();
	}
	if (tokens.back().getName() != "EOF")
		tokens.push_back(Token(currentLine, "", "EOF"));
	return tokens;
}
void LexicalAnalyzer::whiteSpace() {
	while (isspace(crnt) /*&& !in.eof()*/) {
		if (crnt == '\n')
			currentLine++;
		crnt = in.get();
		in.peek();
	}
}
void LexicalAnalyzer::colonDash() {
	if (in.peek() == '-') {
		in.get();
		tokens.push_back(Token(beginingLine, ":-", "COLON_DASH"));
	} else {
		tokens.push_back(Token(beginingLine, ":", "COLON"));
	}
}
void LexicalAnalyzer::comment() {
	string comment;
	comment += crnt;
	while (in.peek() != '\n' && (!in.eof()))
		comment += in.get();
}
void LexicalAnalyzer::strings() {
	string litstring;
	litstring += crnt;
	while (!in.eof() && in.peek() != '\n') {
		litstring += in.get();
		if (litstring.back() == '\'') {
			break;
		}
		in.peek();
	}
	if (litstring.back() != '\'') {
		tokens.push_back(Token(beginingLine, litstring, "UNDEFINED"));
	} else {
		tokens.push_back(Token(beginingLine, litstring, "STRING"));
	}
}
void LexicalAnalyzer::clearID() {

	if (in.eof()) {
		tokens.push_back(Token(beginingLine, "", "EOF"));
		return;
	}
	string multi;
	multi += crnt;
	if (!isalpha(crnt)) {
		tokens.push_back(Token(beginingLine, multi, "UNDEFINED"));
		return;
	}
	while (isalpha(in.peek()) || isdigit(in.peek())) {
		multi += in.get();
	}
	keyword(multi);
}
void LexicalAnalyzer::keyword(string id) {
	if (id == "Schemes") {
		tokens.push_back(Token(beginingLine, id, "SCHEMES"));
	} else if (id == "Facts") {
		tokens.push_back(Token(beginingLine, id, "FACTS"));
	} else if (id == "Rules") {
		tokens.push_back(Token(beginingLine, id, "RULES"));
	} else if (id == "Queries") {
		tokens.push_back(Token(beginingLine, id, "QUERIES"));
	} else {
		tokens.push_back(Token(beginingLine, id, "ID"));
	}
}
Token LexicalAnalyzer::getToken() {
	//cout << "LexicalAnalyzer::getToken()" << endl;
	if (in.eof()) {
		return tokens.back();
	}
	crnt = in.get();
	in.peek();
	whiteSpace();
	beginingLine = currentLine;
	//cout << "switch(" << crnt << ')' << endl;
	switch (crnt) {
	case ',':
		//cout << "	case ',':" << endl;
		tokens.push_back(Token(beginingLine, ",", "COMMA"));
		return tokens.back();
	case '.':
		//cout << "	case '.':" << endl;
		tokens.push_back(Token(beginingLine, ".", "PERIOD"));
		return tokens.back();
	case '?':
		//cout << "	case '?':" << endl;
		tokens.push_back(Token(beginingLine, "?", "Q_MARK"));
		return tokens.back();
	case '(':
		//cout << "	case '(':" << endl;
		tokens.push_back(Token(beginingLine, "(", "LEFT_PAREN"));
		return tokens.back();
	case ')':
		//cout << "	case ')':" << endl;
		tokens.push_back(Token(beginingLine, ")", "RIGHT_PAREN"));
		return tokens.back();
	case ':':
		//cout << "	case ':':" << endl;
		colonDash();
		return tokens.back();
	case '#': {
		//cout << "	case '#':" << endl;
		comment();
		return Token();
	}
	case '\'': {
		//cout << "	case '\'':" << endl;
		strings();
		return tokens.back();
	}
	default:
		//cout << "	default:" << endl;
		clearID();
		return tokens.back();
	}
}

