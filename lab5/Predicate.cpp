/*
 * Predicate.cpp
 *
 *  Created on: May 14, 2015
 *      Author: jacobmb
 */

#include <string>
#include <vector>
#include <sstream>

#include "Predicate.h"
#include "Parameter.h"
#include "Token.h"
#include "Scheme.h"
#include "Tuple.h"

string Predicate::toString() {
	stringstream ss;
	ss << id.getLiteral() << '(';
	for (int i = 0; i < paramList.size(); i++) {
		ss << paramList[i].toString();
		if (i < paramList.size() - 1)
			ss << ',';
	}
	ss << ")";
	return ss.str();
}
Scheme Predicate::toScheme() {
	Scheme scheme;
	scheme.setName(getIDLiteral());
	for (Parameter param : getParamList()) {
		scheme.push_back(param.getLiteral());
	}
	return scheme;
}
vector<Scheme> Predicate::toScheme(vector<Predicate> predicates) {
	vector<Scheme> schemes;
	for (Predicate predicate : predicates) {
		schemes.push_back(predicate.toScheme());
	}
	return schemes;
}
Tuple Predicate::toTuple() {
	Tuple tuple;
	tuple.setName(getIDLiteral());
	for (Parameter param :getParamList()) {
		tuple.push_back(param.getLiteral());
	}
	return tuple;
}
vector<Tuple> Predicate::toTuple(vector<Predicate> predicates) {
	vector<Tuple> tuples;
	for (Predicate predicate : predicates) {
		tuples.push_back(predicate.toTuple());
	}
	return tuples;
}

