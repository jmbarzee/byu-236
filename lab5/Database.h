/*
 * Database.h
 *
 *  Created on: Jul 22, 2015
 *      Author: jacobmb
 */

#ifndef DATABASE_H_
#define DATABASE_H_

using namespace std;

#include <string>
#include <map>
#include <vector>
#include <sstream>

#include "Relation.h"
#include "Scheme.h"
#include "Tuple.h"
#include "DataLog.h"
#include "Predicate.h"
#include "RuleTree.h"

class Database {
private:
	map<string, Relation> relations;
	ostream* out;
	vector<Predicate> queries;
	RuleTree nextRules;
	stringstream* ss = new stringstream();
	bool stable = false;
public:
	Database(DataLog dataLog, ostream* out);
	~Database() {
		delete ss;
	}
private:
	void evaluateQueries();
	void evaluateRules();
	void printRelations();

	vector <Relation> getRelations(vector <Scheme> schemes);
	Relation getOriginal(Relation relation0, Relation relation1);

	void select(Relation &relation, Scheme &query);
	void project(Relation &relation, Scheme &query);
	void rename(Relation &relation, Scheme &query);

	Relation join(Relation relation0, Relation relation1);
	Relation Union(Relation relation0, Relation relation1);

	void projectJoined(Relation &relation, Scheme &query);
};

#endif /* DATABASE_H_ */
