/*
 * Predicate.h
 *
 *  Created on: May 14, 2015
 *      Author: jacobmb
 */

#ifndef PREDICATE_H_
#define PREDICATE_H_

#include <string>
#include <vector>

#include "Token.h"
#include "Parameter.h"

using namespace std;

class Predicate {
public:
	Predicate(Token id = Token()) {
		this->id = id;
	}
	~Predicate(){}
	int getIDLine() {
		return id.getLine();
	}
	string getIDLiteral() {
		return id.getLiteral();
	}
	string getIDName() {
		return id.getName();
	}
	vector<Parameter> getParamList() {
		return paramList;
	}
	void addParam(Parameter param) {
		paramList.push_back(param);
	}
	void setParamList(vector<Parameter> paramList) {
		this->paramList = paramList;
	}
	string toString();
private:
	Token id;
	vector<Parameter> paramList;
};

#endif /* PREDICATE_H_ */
