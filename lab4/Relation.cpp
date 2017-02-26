/*
 * Relation.cpp
 *
 *  Created on: Jul 27, 2015
 *      Author: jacobmb
 */

#include "Relation.h"

#include <string>
#include <sstream>
#include <iostream>

using namespace std;

string Relation::toString() {
	stringstream ss;
	ss.str("");
	for (Tuple tuple : truths) {
		if (tuple.size() > 0)
			ss << "  ";
		for (int i = 0; i < tuple.size(); i++) {
			ss << scheme[i] << "=" << tuple.at(i);
			if (i < tuple.size() - 1)
				ss << " ";
		}
		if (tuple.size() > 0)
			ss << endl;
	}
	return ss.str();
}

