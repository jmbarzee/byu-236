/*
 * Parser.h
 *
 *  Created on: May 14, 2015
 *      Author: jacobmb
 */

#ifndef PARSER_H_
#define PARSER_H_

#include <string>
#include <vector>

#include "Token.h"
#include "Predicate.h"
#include "DataLog.h"

using namespace std;

class Parser {
public:
	Parser(vector<Token> tokens) {
		this->tokens = tokens;
		pos = -1;
		lastToken = Token();
	}
	;
	~Parser() {
	}
	;
	bool parse();
	Token getLastToken() {
		return lastToken;
	}
	DataLog getDataLog() {
		return dataLog;
	}
private:
	vector<Token> tokens;
	DataLog dataLog;
	Token lastToken;
	int pos;

	Token crnt();
	Token next();
	Token peek(int extra = 0);

	void schemeMeta();
	void factMeta();
	void ruleMeta();
	void queryMeta();

	void schemeList();
	void scheme();
	void factList();
	void fact();
	void ruleList();
	void rule();
	void queryList();
	void query();

	void predicateList(vector<Predicate>& predicateList);
	Predicate predicate();
	void parameterList(vector<Parameter>& paramList);
	Parameter parameter();

};

#endif /* PARSER_H_ */
