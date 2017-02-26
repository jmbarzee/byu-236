/*
 * DependancyGraph.h
 *
 *  Created on: Aug 6, 2015
 *      Author: jacobmb
 */

#ifndef DEPENDANCYGRAPH_H_
#define DEPENDANCYGRAPH_H_

#include <set>
#include <vector>
#include <map>
#include <queue>
#include <deque>
#include <ostream>

#include "Edge.h"
#include "Rule.h"
#include "RuleNode.h"
#include "RuleTree.h"
#include "RefinedRule.h"

using namespace std;

class DependancyGraph {
private:
	map<int, RefinedRule> ruleIndex;
	int totalRules;
	vector<Edge> dependancies;

	queue<int> order;

	map<int, RuleNode> graph;
	map<int, RuleNode> reverseGraph;

	deque<RuleTree> results = deque<RuleTree>();

	ostream * out;
public:
	DependancyGraph(vector<RefinedRule> &rules, ostream * out);
private:
	int findRule(Scheme scheme);
	int findRule(RefinedRule rule);
	vector<int> findRules(Scheme scheme);

	void buildDependancies(const vector<RefinedRule> &rules);

	void buildGraph();
	void buildReverseGraph();

	void printGraph();
	void printReverseGraph();
	void printOrder();
	void printSCC();

	void dfsForestReverse();
	void dfsReverse(RuleNode &ruleNode);

	void dfsForest();
	void dfs(RuleNode &ruleNode);

public:
	deque<RuleTree> getResults() {
		return results;
	}
};

#endif /* DEPENDANCYGRAPH_H_ */
