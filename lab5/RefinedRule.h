/*
 * RefinedRule.h
 *
 *  Created on: May 14, 2015
 *      Author: jacobmb
 */

#ifndef REFINEDRULE_H_
#define REFINEDRULE_H_

#include <string>
#include <vector>

#include "Scheme.h"

using namespace std;

class RefinedRule {
public:
	RefinedRule(Scheme scheme) {
		this->scheme = scheme;
	}
	~RefinedRule() {
	}
	Scheme getHeadScheme() {
		return scheme;
	}
	vector<Scheme> getSchemeList() {
		return schemeList;
	}
	void setSchemeList(vector<Scheme> schemeList) {
		this->schemeList = schemeList;
	}
	string toString();
	bool isSelfDependant();
	bool isSame(RefinedRule rule);
private:
	Scheme scheme;
	vector<Scheme> schemeList;
};

#endif /* REFINEDRULE_H_ */
