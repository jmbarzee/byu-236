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

using namespace std;

string Rule::toString() {
	stringstream ss;
	ss << predicate.toString() << " :- ";
	for (int i=0; i < predicateList.size(); i++) {
		ss << predicateList[i].toString();
		if (i < predicateList.size()-1)
			ss << ',';
	}
	ss << "\n";
	return ss.str();
}

