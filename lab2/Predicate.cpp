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

string Predicate::toString() {
	stringstream ss;
	ss << id.getLiteral() << '(';
	for (int i=0; i < paramList.size(); i++) {
		ss << paramList[i].toString();
		if (i < paramList.size()-1)
			ss << ',';
	}
	ss << ")";
	return ss.str();
}



