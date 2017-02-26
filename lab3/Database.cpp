/*
 * Database.cpp
 *
 *  Created on: Jul 23, 2015
 *      Author: jacobmb
 */

#include <vector>
#include <map>
#include <string>
#include <ostream>
#include <fstream>
#include <iostream>

#include "Database.h"
#include "DataLog.h"
#include "Predicate.h"
#include "Parameter.h"
#include "Scheme.h"
#include "Relation.h"
#include "Tuple.h"

using namespace std;

Database::Database(DataLog dataLog, ostream * out) {
	this->out = out;
	vector<Predicate> schemes = dataLog.getSchemes();
	for (int i = 0; i < schemes.size(); i++) {
		string name = schemes[i].getIDLiteral();
		vector<Parameter> parameters = schemes[i].getParamList();
		Scheme scheme;
		for (int j = 0; j < parameters.size(); j++)
			scheme.push_back(parameters[j].getLiteral());
		relations.emplace(name, Relation(name, scheme));
	}
	*out << "Scheme Evaluation" << endl << endl;
	vector<Predicate> facts = dataLog.getFacts();
	for (int i = 0; i < facts.size(); i++) {
		string name = facts[i].getIDLiteral();
		vector<Parameter> parameters = facts[i].getParamList();
		Tuple tuple;
		for (int j = 0; j < parameters.size(); j++)
			tuple.push_back(parameters[j].getLiteral());
		relations.at(name).addTuple(tuple);
	}
	*out << "Fact Evaluation" << endl << endl;
	printRelations();
	//TODO add rules to database
	*out << "Query Evaluation" << endl << endl;
	queries = dataLog.getQueries();
	evaluateQueries();

}
void Database::printRelations() {
	map<string, Relation>::iterator mapit = relations.begin();
	for (mapit; mapit != relations.end(); mapit++) {
		*out << mapit->second.getName() << endl;
		Scheme scheme = mapit->second.getScheme();
		set<Tuple> tuples = mapit->second.getTruths();
		set<Tuple>::iterator setit = tuples.begin();

		for (setit; setit != tuples.end(); setit++) {
			*out << "  ";
			Tuple tuple = *setit;
			for (int i = 0; i < scheme.size(); i++) {
				*out << scheme[i] << '=' << tuple[i] << ' ';
			}
			*out << endl;
		}
		*out << endl;
	}

}
void Database::evaluateQueries() {
	for (int i = 0; i < queries.size(); i++) {
		ss->str("");

		//build results relation filled with tuples that match the querie's name
		Relation results = relations.at(queries[i].getIDLiteral());

		//builds the scheme for the query
		vector<Parameter> parameters = queries[i].getParamList();
		Scheme scheme;
		for (Parameter param : parameters) {
			scheme.push_back(param.getLiteral());
		}
		*out << queries.at(i).toString() << "? ";

		select(results, scheme);
		*ss << "select" << endl << results.toString();

		project(results, scheme);
		*ss << "project" << endl << results.toString();

		rename(results, scheme);
		*ss << "rename" << endl << results.toString();

		if (results.getTruths().size() > 0) {
			*out << "Yes(" << results.getTruths().size() << ')' << endl;
			*out << ss->str() << endl;
		} else {
			*out << "No" << endl << endl;
		}
		ss->clear();
	}
}
void Database::select(Relation &relation, Scheme &query) {
	cout << "Select" << endl << relation.toString() << endl;
	set<Tuple> tuples = relation.getTruths();
	set<Tuple> selection;
	for (Tuple tuple : tuples) {
		cout << "  New tuple" << endl;
		bool viable = true;
		map<string, string> usedVariables;
		for (int i = 0; i < query.size(); i++) {
			cout << "\tchecking: " << query.at(i) << " =? " << tuple.at(i)
					<< endl;
			if (query.at(i).at(0) == '\'' && query.at(i) != tuple.at(i)) {
				cout << "\t  strings don't match, not VIABLE" << endl;
				viable = false;
				break;
			} else if (query.at(i).at(0) != '\'') {
				string var = "";
				try {
					//find if var was used before
					var = usedVariables.at(query.at(i));
					cout << "\t\t repeat: " << var << endl;
				} catch (out_of_range &e) {
					//if not found in map, put it in
					usedVariables.emplace(query.at(i), tuple.at(i));
					var = tuple.at(i);
					cout << "\t\t   new: " << var << endl;
				}
				if (var != tuple.at(i)) {
					cout << "\t  bad repeat, not VIABLE" << endl;
					viable = false;
					break;
				}
			}
		}

		if (viable) {
			cout << "\t  VIABLE, moving to new set" << endl;
			selection.insert(tuple);
		}
	}
	relation.setTruths(selection);
	cout << relation.toString() << endl;
}
void Database::project(Relation &relation, Scheme &query) {
	cout << "Project" << endl;
	Scheme scheme = relation.getScheme();
	set<Tuple> tuples = relation.getTruths();
	set<Tuple> newTuples;

	for (Tuple tuple : tuples) {
		set<string> usedVars;
		Tuple newTuple;
		for (int i = 0; i < query.size(); i++) {
			cout << "  projecting: " << query.at(i) << endl;
			if (query.at(i).at(0) != '\''
					&& usedVars.find(query.at(i)) == usedVars.end()) {
				cout << "\tcopying!" << endl;
				usedVars.insert(query.at(i));
				newTuple.push_back(tuple.at(i));
			}
		}
		newTuples.insert(newTuple);
	}
	for (int i = 0; i < query.size(); i++) {
		set<string> usedVars;
		if (query.at(i).at(0) == '\''
				&& usedVars.find(query.at(i)) == usedVars.end()) {
			cout << "\t  finally erasing: " << *(query.begin() + i) << endl;
			usedVars.insert(query.at(i));
			query.erase(query.begin() + i);
			scheme.erase(scheme.begin() + i);
			i--;
		}
	}
	relation.setScheme(scheme);
	relation.setTruths(newTuples);
	cout << relation.toString();
}
void Database::rename(Relation &relation, Scheme &query) {
	Scheme scheme = relation.getScheme();
	for (int i = 0; i < query.size(); i++) {
		if (query[i] != scheme[i]) {
			scheme[i] = query[i];
		}
	}
	relation.setScheme(scheme);
	cout << relation.toString();
}


