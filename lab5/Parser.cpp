/*
 * Parser.cpp
 *
 *  Created on: May 14, 2015
 *      Author: jacobmb
 */

#include <vector>
#include <iostream>
#include <stdexcept>

#include "Predicate.h"
#include "Parameter.h"
#include "Parser.h"
#include "Token.h"

using namespace std;

Token Parser::crnt() {
	return tokens.at(pos);
}
Token Parser::next() {
	//cout << "			next()" << endl;
	//cout << "			  " << tokens.at(pos+1).toString();
	if (pos + 1 >= tokens.size())
		throw tokens.at(pos);
	pos++;
	if (tokens.at(pos).isType("COMMENT"))
		return next();
	return tokens.at(pos);
}
Token Parser::peek(int extra) {
	//cout << "			peek()" << endl;
	//cout << "			  " << tokens.at(pos + 1).toString();
	if ((pos + extra + 1) >= tokens.size())
		throw tokens.at(pos + extra + 1);
	if (tokens.at(pos + extra + 1).isType("COMMENT"))
		return peek(extra + 1);
	return tokens.at(pos + extra + 1);
}
bool Parser::parse() {
	//cout << "parse()" << endl;
	this->tokens = tokens;
	try {
		schemeMeta();
		factMeta();
		ruleMeta();
		queryMeta();
		if (!peek().isType("EOF"))
			throw next();
		next();
	} catch (Token& lastToken) {
		//cout << "CATCH!" << endl;
		this->lastToken = lastToken;
		return false;
	} catch (out_of_range& oor) {
		//cout << "OUT OF RANGE!" << endl;
		this->lastToken = tokens.back();
		return false;
	}
	if (lastToken.isType("UNDEFINED"))
		this->lastToken = tokens.back();
	return true;
}
void Parser::schemeMeta() {
	if (!peek().isType("SCHEMES"))
		throw next();
	next();
	if (!peek().isType("COLON"))
		throw next();
	next();
	schemeList();
}
void Parser::factMeta() {
	if (!peek().isType("FACTS"))
		throw next();
	next();
	if (!peek().isType("COLON"))
		throw next();
	next();
	factList();
}
void Parser::ruleMeta() {
	if (!peek().isType("RULES"))
		throw next();
	next();
	if (!peek().isType("COLON"))
		throw next();
	next();
	ruleList();
}
void Parser::queryMeta() {
	if (!peek().isType("QUERIES"))
		throw next();
	next();
	if (!peek().isType("COLON"))
		throw next();
	next();
	queryList();
}
void Parser::schemeList() {
	//cout << "	schemeList()" << endl;
	//->  <Scheme> <Scheme List>
	//->  e
	scheme();
	if (!peek().isType("FACTS")) {
		schemeList();
	}
	return;
}
void Parser::scheme() {
	//cout << "		scheme()" << endl;
	//->  <Predicate>
	Predicate scheme = predicate();
	dataLog.addScheme(scheme);
	return;
}
void Parser::factList() {
	//cout << "	factList()" << endl;
	//->  <Fact> <Fact List>
	//->  e
	if (!peek().isType("RULES")) {
		fact();
		factList();
	}
	return;
}
void Parser::fact() {
	//cout << "		fact()" << endl;
	//->  <Predicate>.
	Predicate fact = predicate();
	if (!peek().isType("PERIOD"))
		throw next();
	next();
	dataLog.addFact(fact);
	return;

}
void Parser::ruleList() {
	//cout << "	ruleList()" << endl;
	//->  <Rule> <Rule List>
	//->  e
	if (!peek().isType("QUERIES")) {
		rule();
		ruleList();
	}
	return;
}
void Parser::rule() {
	//cout << "		rule()" << endl;
	//->  <Head Predicate> :- <Predicate List> .
	Rule rule(predicate());
	if (!peek().isType("COLON_DASH"))
		throw next();
	next();
	vector<Predicate> ruleList;
	predicateList(ruleList);
	rule.setPredicateList(ruleList);
	if (!peek().isType("PERIOD"))
		throw next();
	next();
	dataLog.addRule(rule);
	return;
}

void Parser::queryList() {
	//cout << "	queryList()" << endl;
	//->  <Query> <Query List>
	//->  e
	query();
	if (!peek().isType("EOF")) {
		queryList();
	}
	return;
}
void Parser::query() {
	//cout << "		query()" << endl;
	//->  <Predicate> ?
	Predicate query = predicate();
	if (!peek().isType("Q_MARK"))
		throw next();
	next();
	dataLog.addQuery(query);
	return;
}
void Parser::predicateList(vector<Predicate>& predicateVector) {
	//cout << "		predicateList()" << endl;
	//->  <Predicate> , <Predicate List>
	//->  <Predicate>
	predicateVector.push_back(predicate());
	if (peek().isType("COMMA")) {
		next();
		predicateList(predicateVector);
	}
}
Predicate Parser::predicate() {
	//cout << "		predicate()" << endl;
	//->  Identifier ( <Parameter List> )
	if (!peek().isType("ID"))
		throw next();
	Predicate predicate = Predicate(next());
	if (!peek().isType("LEFT_PAREN"))
		throw next();
	next();
	vector<Parameter> paramList;
	parameterList(paramList);
	predicate.setParamList(paramList);
	if (!peek().isType("RIGHT_PAREN"))
		throw next();
	next();
	return predicate;
}
void Parser::parameterList(vector<Parameter>& paramList) {
	//cout << "		parameterList()" << endl;
	//->  <Parameter> , <Parameter List>
	//->  <Parameter>
	paramList.push_back(parameter());
	if (peek().isType("COMMA")) {
		next();
		parameterList(paramList);
	}
	return;
}
Parameter Parser::parameter() {
	//->  String | Identifier
	//cout << "		parameter()" << endl;
	if (peek().isType("STRING"))
		dataLog.addToDomain(peek().getLiteral());
	if (!peek().isType("STRING") && !peek().isType("ID"))
		throw next();
	return Parameter(next());
}
