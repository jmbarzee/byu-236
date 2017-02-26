/*
 * Scheme.h
 *
 *  Created on: Jul 22, 2015
 *      Author: jacobmb
 */

#ifndef SCHEME_H_
#define SCHEME_H_

#include <vector>
#include <string>
#include <sstream>

using namespace std;

class Scheme: public vector<string> {
private:
	string name = "";
public:
	string const toString() {
		stringstream ss;
		for (int i = 0; i < this->size(); i++) {
			ss << this->at(i) << ' ';
		}
		ss << endl;
		return ss.str();
	}
	void setName(string name) {
		this->name = name;
	}
	string getName() {
		return name;
	}
	//used this instead of double equals because it is not testing equivalence
	bool isSame(Scheme scheme) {
		if (this->name != scheme.getName())
			return false;
		if (this->size() != scheme.size())
			return false;
		return true;
	}
};

#endif /* SCHEME_H_ */
