/*
 * Rule.cpp
 *
 *  Created on: May 14, 2015
 *      Author: jacobmb
 */

#include <string>
#include <vector>
#include <sstream>

#include "RefinedRule.h"

using namespace std;

string RefinedRule::toString() {
	stringstream ss;
	ss << scheme.toString() << " :- ";
	for (int i=0; i < schemeList.size(); i++) {
		ss << schemeList.at(i).toString();
		if (i < schemeList.size()-1)
			ss << ',';
	}
	return ss.str();
}
bool RefinedRule::isSelfDependant() {
	string name = this->scheme.getName();
	for(Scheme scheme : schemeList) {
		if (name == scheme.getName())
			return true;
	}
	return false;
}
bool RefinedRule::isSame(RefinedRule rule) {
	if (!this->getHeadScheme().isSame(rule.getHeadScheme()))
		return false;
	vector <Scheme> schemeList = rule.getSchemeList();
	if (schemeList.size() != this->schemeList.size())
		return false;
	for(int i = 0; i<schemeList.size(); i++) {
		if (!this->schemeList.at(i).isSame(schemeList.at(i)))
			return false;
	}
	return true;
}

