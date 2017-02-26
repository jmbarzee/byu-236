/*
 * Rule.cpp
 *
 *  Created on: May 14, 2015
 *      Author: jacobmb
 */

#include <string>
#include <vector>
#include <sstream>

#include "Predicate.h"
#include "Rule.h"
#include "RefinedRule.h"

using namespace std;

string Rule::toString() {
	stringstream ss;
	ss << predicate.toString() << " :- ";
	for (int i = 0; i < predicateList.size(); i++) {
		ss << predicateList[i].toString();
		if (i < predicateList.size() - 1)
			ss << ',';
	}
	ss << "\n";
	return ss.str();
}
bool Rule::isSelfDependant() {
	string name = this->predicate.getIDLiteral();
	for (Predicate predicate : predicateList) {
		if (name == predicate.getIDLiteral())
			return true;
	}
	return false;
}
RefinedRule Rule::refine() {
	RefinedRule rule = RefinedRule(getHeadPredicate().toScheme());
	rule.setSchemeList(Predicate::toScheme(getPredicateList()));
	return rule;
}
vector<RefinedRule> Rule::refine(vector<Rule> rules) {
	vector <RefinedRule> newRules;
	for (Rule rule : rules) {
		newRules.push_back(rule.refine());
	}
	return newRules;
}

