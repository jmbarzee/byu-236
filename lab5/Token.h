/*
 * Token.h
 *
 *  Created on: Mar 23, 2015
 *      Author: jacobmb
 */

#ifndef Token_H_
#define Token_H_

#include <string>
#include <ostream>

using namespace std;

class Token {
private:
	int line;
	string literal;
	string name;
public:
	Token(int line = -1, string literal = "", string name = "UNDEFINED") {
		this->line = line;
		this->literal = literal;
		this->name = name;
	}
	~Token(){}
	int getLine() { return line; }
	string getLiteral() { return literal;}
	string getName() {return name;}
	string toString();
	bool isType(string name);
};

#endif /* Token_H_ */
