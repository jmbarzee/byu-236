/*
 * tuple.h
 *
 *  Created on: Jul 22, 2015
 *      Author: jacobmb
 */

#ifndef TUPLE_H_
#define TUPLE_H_

#include <vector>
#include <string>
#include <sstream>

using namespace std;

class Tuple: public vector<string> {
public:
	string const toString() {
		stringstream ss;
		for (int i = 0; i<this->size(); i++) {
			ss << this->at(i) << ' ';
		}
		ss << endl;
		return ss.str();
	}

};

#endif /* TUPLE_H_ */
