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

class Database {
private:
	map<string, Relation> relations;
	ostream* out;
	vector<Predicate> queries;
	stringstream* ss = new stringstream();
public:
	Database(DataLog dataLog, ostream* out);
	~Database() {
		delete ss;
	}
private:
	void printRelations();
	void evaluateQueries();
	void select(Relation &relation, Scheme &query);
	void project(Relation &relation, Scheme &query);
	void rename(Relation &relation, Scheme &query);
};

#endif /* DATABASE_H_ */
