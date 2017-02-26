/*
 * RefinedRuleNode.h
 *
 *  Created on: Aug 4, 2015
 *      Author: jacobmb
 */

#ifndef RULENODE_H_
#define RULENODE_H_

#include <set>
#include <string>
#include <sstream>

#include "RefinedRule.h"

using namespace std;

class RuleNode {
private:
	int index;
	bool visited;
	RefinedRule rule;
	set<int> prereqs;
public:
	RuleNode(int index, RefinedRule rule) :
			rule(rule), index(index), visited(false) {
	}
	int getIndex() {
		return index;
	}
	bool isVisited() {
		return visited;
	}
	RefinedRule getRule() {
		return rule;
	}
	set<int> getPrereqs() {
		return prereqs;
	}
	void setVisited(bool visited) {
		this->visited = visited;
	}
	void addPrereq(int newPrereq) {
		prereqs.insert(newPrereq);
	}
	const string toString() {
		stringstream ss;
		ss << "R" << index << ":";
		for (int index : prereqs) {
			ss << " R" << index;
		}
		ss << endl;
		return ss.str();
	}
};

#endif /* RULENODE_H_ */
