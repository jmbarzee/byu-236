/*
 * DataLog.cpp
 *
 *  Created on: May 14, 2015
 *      Author: jacobmb
 */

#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <sstream>

#include "DataLog.h"
#include "Token.h"
#include "Parameter.h"
#include "Predicate.h"
#include "Rule.h"

using namespace std;

string DataLog::toString() {
	stringstream ss;
	//cout << "schemes" << endl;
	ss << "Schemes(" << schemes.size() << "):\n";
	for (int i = 0; i < schemes.size(); i++) {
		ss << "  " << schemes[i].toString() << '\n';
	}
	//cout << "facts" << endl;
	ss << "Facts(" << facts.size() << "):\n";
	for (int i = 0; i < facts.size(); i++) {
		ss << "  " << facts[i].toString() << "\n";
	}
	//cout << "rules" << endl;
	ss << "Rules(" << rules.size() << "):\n";
	for (int i = 0; i < rules.size(); i++) {
		ss << "  " << rules[i].toString();
	}
	//cout << "Queries" << endl;
	ss << "Queries(" << queries.size() << "):\n";
	for (int i = 0; i < queries.size(); i++) {
		ss << "  " << queries[i].toString() << "\n" ;
	}
	//cout << "Domain" << endl;
	ss << "Domain(" << domain.size() << "):\n";
	for (set<string>::iterator i = domain.begin(); i != domain.end(); i++) {
		ss << "  " << *i << '\n';
	}
	return ss.str();
}

