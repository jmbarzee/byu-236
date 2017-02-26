/*
 * Parameter.h
 *
 *  Created on: May 14, 2015
 *      Author: jacobmb
 */

#ifndef PARAMETER_H_
#define PARAMETER_H_

#include <string>

#include "Token.h"

using namespace std;

class Parameter {
public:
	Parameter(Token token) {
		this->token = token;
	}
	~Parameter(){}
	int getLine() {
		return token.getLine();
	}
	string getLiteral() {
		return token.getLiteral();
	}
	string getName() {
		return token.getName();
	}
	string toString();
private:
	Token token;
};

#endif /* PARAMETER_H_ */
