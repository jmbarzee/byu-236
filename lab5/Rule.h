/*
 * Rule.h
 *
 *  Created on: May 14, 2015
 *      Author: jacobmb
 */

#ifndef RULE_H_
#define RULE_H_

#include <string>
#include <vector>

#include "RefinedRule.h"
#include "Predicate.h"

using namespace std;

class Rule {
public:
	Rule(Predicate predicate) {
		this->predicate = predicate;
	}
	~Rule() {
	}
	Predicate getHeadPredicate() {
		return predicate;
	}
	vector<Predicate> getPredicateList() {
		return predicateList;
	}
	void setPredicateList(vector<Predicate> predicateList) {
		this->predicateList = predicateList;
	}
	string toString();
	bool isSelfDependant();
	RefinedRule refine();
	static vector<RefinedRule> refine(vector <Rule> rules);
private:
	Predicate predicate;
	vector<Predicate> predicateList;
};

#endif /* RULE_H_ */
