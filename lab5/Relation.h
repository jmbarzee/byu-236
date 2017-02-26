/*
 * Relation.h
 *
 *  Created on: Jul 22, 2015
 *      Author: jacobmb
 */

#ifndef RELATION_H_
#define RELATION_H_

#include <set>
#include <string>
#include <iostream>

#include "Tuple.h"
#include "Scheme.h"

using namespace std;

class Relation {
private:
	string name;
	Scheme scheme;
	set<Tuple> truths;
public:
	Relation(string name, Scheme scheme) {
		this->name = name;
		this->scheme = scheme;
	}
	Relation() {
		this->name = "";
	}
	string getName() {
		return name;
	}
	Scheme getScheme() {
		return scheme;
	}
	set<Tuple> getTruths() {
		return truths;
	}
	int getSize() {
		return truths.size();
	}
	void addTuple(Tuple truth) {
		truths.insert(truth);
	}
	void setName(string name) {
		this->name = name;
	}
	void setScheme(Scheme scheme) {
		this->scheme = scheme;
	}
	void setTruths(set<Tuple> truths) {
		this->truths = truths;
	}
	string toString();
};

#endif /* RELATION_H_ */
