/*
 * DataLog.h
 *
 *  Created on: May 14, 2015
 *      Author: jacobmb
 */

#ifndef DATALOG_H_
#define DATALOG_H_

#include <string>
#include <vector>
#include <set>

#include "Token.h"
#include "Parameter.h"
#include "Predicate.h"
#include "Rule.h"

using namespace std;

class DataLog {
public:
	void addScheme(Predicate scheme) {
		schemes.push_back(scheme);
	}
	void addFact(Predicate fact) {
		facts.push_back(fact);
	}
	void addRule(Rule rule) {
		rules.push_back(rule);
	}
	void addQuery(Predicate querie) {
		queries.push_back(querie);
	}
	void addToDomain(string id) {
		domain.insert(id);
	}
	vector<Predicate> getSchemes() {
		return schemes;
	}
	vector<Predicate> getFacts() {
		return facts;
	}
	vector<Rule> getRules() {
		return rules;
	}
	vector<Predicate> getQueries() {
		return queries;
	}
	set<string> getDomain() {
		return domain;
	}
	string toString();
private:
	vector<Predicate> schemes;
	vector<Predicate> facts;
	vector<Rule> rules;
	vector<Predicate> queries;
	set<string> domain;
};

#endif /* DATALOG_H_ */
