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
#include <deque>

#include "Database.h"
#include "DataLog.h"
#include "Predicate.h"
#include "Parameter.h"
#include "Scheme.h"
#include "Relation.h"
#include "Tuple.h"
#include "DependancyGraph.h"
#include "RuleTree.h"

using namespace std;

Database::Database(DataLog dataLog, ostream * out) {
	this->out = out;
	*out << "Scheme Evaluation" << endl << endl;
	vector<Scheme> schemes = Predicate::toScheme(dataLog.getSchemes());
	for (Scheme scheme : schemes) {
		string name = scheme.getName();
		relations.emplace(name, Relation(name, scheme));
	}
	*out << "Fact Evaluation" << endl << endl;
	vector<Tuple> tuples = Predicate::toTuple(dataLog.getFacts());
	for (Tuple tuple : tuples) {
		string name = tuple.getName();
		relations.at(name).addTuple(tuple);
	}
	printRelations();

	*out << "Rule Evaluation" << endl << endl;
	vector<RefinedRule> rules = Rule::refine(dataLog.getRules());
	DependancyGraph dependancyGraph = DependancyGraph(rules, out);
	/*deque<RuleTree> ruleTrees = dependancyGraph.getResults();
	for (RuleTree ruleTree : ruleTrees) {
		nextRules = ruleTree;
		while (!stable) {
			evaluateRules();
			if (ruleTree.size() == 1 && !ruleTree.at(0).isSelfDependant())
				stable = true;
		}
	}*/
	*out << endl;
	*out << endl;
	printRelations();

	*out << "Query Evaluation" << endl << endl;
	//cout << "Query Evaluation" << endl << endl;
	queries = dataLog.getQueries();
	evaluateQueries();

}

void Database::evaluateRules() {
	stable = true;
	for (RefinedRule rule : nextRules) {
		*out << rule.toString();
		Scheme conclusion = rule.getHeadScheme();
		vector<Scheme> premises = rule.getSchemeList();
		vector<Relation> relations;
		for (Scheme premise : premises) {
			Relation relation = this->relations.at(premise.getName());
			select(relation, premise);
			relations.push_back(relation);
		}
		Relation newRelation;
		if (premises.size() == 1) {
			newRelation = relations.at(0);
			rename(newRelation, premises.at(0));
			select(newRelation, premises.at(0));
			projectJoined(newRelation, conclusion);
			Scheme trueScheme =
					this->relations.at(conclusion.getName()).getScheme();
			rename(newRelation, trueScheme);

		} else {
			while (premises.size() > 1) {
				Relation relation0 = relations.at(0);
				Relation relation1 = relations.at(1);
				Scheme scheme0 = premises.at(0);
				Scheme scheme1 = premises.at(1);
				premises.erase(premises.begin());
				premises.erase(premises.begin());
				relations.erase(relations.begin());
				relations.erase(relations.begin());
				relation0.setScheme(scheme0);
				relation1.setScheme(scheme1);
				Relation natJoin = join(relation0, relation1);
				//cout << natJoin.getScheme().toString() << endl;
				//cout << natJoin.toString() << endl;
				relations.push_back(natJoin);
				premises.push_back(natJoin.getScheme());
			}
			Scheme newScheme =
					this->relations.at(conclusion.getName()).getScheme();
			newRelation = relations.at(0);
			select(newRelation, conclusion);
			projectJoined(newRelation, conclusion);
			//cout << "Projected" << endl;
			//cout << "\t" << conclusion.toString();
			//cout << "\t" << newRelation.getScheme().toString();
			//cout << "\t" << newRelation.getName() << endl;
			//cout << newRelation.toString() << endl << endl;
			rename(newRelation, conclusion);
			newRelation.setScheme(newScheme);
		}

		Relation oldRelation = this->relations.at(newRelation.getName());
		newRelation = getOriginal(newRelation, oldRelation);
		*out << newRelation.toString();
		if (newRelation.getTruths().size() != 0)
			stable = false;
		this->relations.erase(newRelation.getName());
		this->relations.emplace(newRelation.getName(),
				Union(newRelation, oldRelation));
	}
}

void Database::evaluateQueries() {
	for (int i = 0; i < queries.size(); i++) {
		ss->str("");

		//build results relation filled with tuples that match the querie's name
		Relation results = relations.at(queries[i].getIDLiteral());

		//builds the scheme for the query
		Scheme scheme = queries[i].toScheme();

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
Relation Database::getOriginal(Relation relation0, Relation relation1) {
	set<Tuple> originalTuples;
	if (relation0.getName() == relation1.getName()) {
		set<Tuple> newTuples = relation0.getTruths();
		set<Tuple> oldTuples = relation1.getTruths();
		for (Tuple tuple : newTuples) {
			if (oldTuples.find(tuple) == oldTuples.end())
				originalTuples.insert(tuple);
		}
	} else {
		throw logic_error("relation names don't match");
	}
	relation0.setTruths(originalTuples);
	return relation0;
}

vector<Relation> Database::getRelations(vector<Scheme> schemes) {
	vector<Relation> relations;
	for (Scheme scheme : schemes) {
		relations.push_back(this->relations.at(scheme.getName()));
	}
	return relations;
}

void Database::select(Relation &relation, Scheme &query) {
	set<Tuple> tuples = relation.getTruths();
	set<Tuple> selection;
	for (Tuple tuple : tuples) {
		bool viable = true;
		map<string, string> usedVariables;
		for (int i = 0; i < query.size(); i++) {
			if (query.at(i).at(0) == '\'' && query.at(i) != tuple.at(i)) {
				viable = false;
				break;
			} else if (query.at(i).at(0) != '\'') {
				string var = "";
				try {
					//find if var was used before
					var = usedVariables.at(query.at(i));
				} catch (out_of_range &e) {
					//if not found in map, put it in
					usedVariables.emplace(query.at(i), tuple.at(i));
					var = tuple.at(i);
				}
				if (var != tuple.at(i)) {
					viable = false;
					break;
				}
			}
		}
		if (viable) {
			selection.insert(tuple);
		}
	}
	relation.setTruths(selection);
}
void Database::project(Relation &relation, Scheme &query) {
	//cout << "Project" << endl;
	Scheme scheme = relation.getScheme();
	set<Tuple> tuples = relation.getTruths();
	set<Tuple> newTuples;

	for (Tuple tuple : tuples) {
		set<string> usedVars;
		Tuple newTuple;
		for (int i = 0; i < query.size(); i++) {
			//cout << "  projecting: " << query.at(i) << endl;
			if (query.at(i).at(0) != '\''
					&& usedVars.find(query.at(i)) == usedVars.end()) {
				//cout << "\tcopying!" << endl;
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
			//cout << "\t  finally erasing: " << *(query.begin() + i) << endl;
			usedVars.insert(query.at(i));
			query.erase(query.begin() + i);
			scheme.erase(scheme.begin() + i);
			i--;
		}
	}
	relation.setScheme(scheme);
	relation.setTruths(newTuples);
	//cout << relation.toString();
}
void Database::rename(Relation &relation, Scheme &query) {
	Scheme scheme = relation.getScheme();
	if (query.size() == scheme.size()) {
		for (int i = 0; i < query.size(); i++) {
			if (query[i] != scheme[i]) {
				scheme[i] = query[i];
			}
		}
	}
	relation.setScheme(scheme);
}

Relation Database::join(Relation relation0, Relation relation1) {
	//cout << "\t\t\tJOINING" << endl;
	//cout << "\t\tRelation0 - " << relation0.getName() << endl;
	//cout << relation0.toString() << endl;
	//cout << "\t\tRelation1 - " << relation1.getName() << endl;
	//cout << relation1.toString() << endl;
	set<Tuple> resultingTuples;
	set<Tuple> tuples0 = relation0.getTruths();
	set<Tuple> tuples1 = relation1.getTruths();
	Scheme resultingScheme;
	Scheme scheme0 = relation0.getScheme();
	Scheme scheme1 = relation1.getScheme();
	for (Tuple tuple0 : tuples0) {
		//cout << "\t using tuple0" << endl;
		//cout << "\t" << tuple0.toString();
		for (Tuple tuple1 : tuples1) {
			//cout << "\t using tuple1" << endl;
			//cout << "\t" << tuple1.toString();
			map<string, string> usedVars;
			Tuple newTuple;
			Scheme newScheme;
			bool viable = true;
			for (int i = 0; i < scheme0.size(); i++) {
				usedVars.emplace(scheme0.at(i), tuple0.at(i));
				newTuple.push_back(tuple0.at(i));
				newScheme.push_back(scheme0.at(i));
				//cout << "\t\t search: " << scheme0.at(i) << endl;
				//cout << "\t\t new: " << tuple0.at(i) << endl;
			}
			for (int i = 0; i < scheme1.size(); i++) {
				string var = "";
				try {
					//find if var was used before
					//cout << "\t\t search: " << scheme1.at(i) << endl;
					var = usedVars.at(scheme1.at(i));
					//cout << "\t\t repeat: " << var << endl;
				} catch (out_of_range &e) {
					//if not found in map, put it in
					usedVars.emplace(scheme1.at(i), tuple1.at(i));
					newTuple.push_back(tuple1.at(i));
					newScheme.push_back(scheme1.at(i));
					var = tuple1.at(i);
					//cout << "\t\t new: " << var << endl;
				}
				if (var != tuple1.at(i)) {
					//cout << "\t  bad repeat, not VIABLE" << endl;
					viable = false;
				}
			}
			if (viable) {
				//cout << "\tJoined Tuple is viable, adding to set" << endl;
				//TODO Remove or comment out this section before pass off
				resultingScheme = newScheme;
				resultingTuples.insert(newTuple);
				resultingScheme = newScheme;
			}
		}
	}
	Relation resultingRelation = Relation("", resultingScheme);
	resultingRelation.setTruths(resultingTuples);
	return resultingRelation;
}
Relation Database::Union(Relation relation0, Relation relation1) {
	set<Tuple> tuplesAll;
	if (relation0.getName() == relation1.getName()) {
		tuplesAll = relation0.getTruths();
		set<Tuple> tuples1 = relation1.getTruths();
		tuplesAll.insert(tuples1.begin(), tuples1.end());
	} else {
		throw logic_error("relation names don't match");
	}
	relation0.setTruths(tuplesAll);
	return relation0;
}
void Database::projectJoined(Relation &relation, Scheme &query) {
	string name = query.getName();

	Scheme scheme = relation.getScheme();
	Scheme newScheme;
	newScheme.setName(name);

	set<Tuple> tuples = relation.getTruths();
	set<Tuple> newTuples;

	vector<int> positionsToTake;
	for (string var : query) {
		if (var[0] != '\'') {
			for (int i = 0; i < scheme.size(); i++) {
				if (var == scheme.at(i)) {
					positionsToTake.push_back(i);
					newScheme.push_back(scheme.at(i));
				}
			}
		}
	}

	for (Tuple tuple : tuples) {
		Tuple newTuple;
		newTuple.setName(query.getName());
		for (int pos : positionsToTake) {
			newTuple.push_back(tuple.at(pos));
		}
		newTuple.setName(name);
		newTuples.insert(newTuple);
	}

	relation.setName(name);
	relation.setScheme(newScheme);
	relation.setTruths(newTuples);
}

