/*
 * DependancyGraph.cpp
 *
 *  Created on: Aug 6, 2015
 *      Author: jacobmb
 */

#include <vector>
#include <map>
#include <deque>
#include <ostream>

#include "Edge.h"
#include "Rule.h"
#include "RuleNode.h"
#include "Database.h"
#include "Scheme.h"
#include "DependancyGraph.h"
#include "RefinedRule.h"

using namespace std;

DependancyGraph::DependancyGraph(vector<RefinedRule> &rules, ostream * out) {
	this->out = out;
	totalRules = rules.size();
	for (int i = 0; i < rules.size(); i++) {
		ruleIndex.emplace(i, rules.at(i));
	}
	/*
	for (int i = 0; i < totalRules; i++) {
		try {
			RefinedRule rule = ruleIndex.at(i);
			*out << "ruleIndex -> " << i << ' ' << rule.toString() << endl;
		} catch (out_of_range& e) {

		}
	}
	*out << ruleIndex.size() << endl;
	*/
	buildDependancies(rules);
	buildGraph();
	printGraph();
	buildReverseGraph();
	printReverseGraph();
	dfsForestReverse();
	printOrder();
	dfsForest();
	printSCC();
}

int DependancyGraph::findRule(Scheme scheme) {
	for (int i = 0; i < ruleIndex.size(); i++) {
		Scheme toCheck = ruleIndex.at(i).getHeadScheme();
		if (scheme.getName() == toCheck.getName() && scheme.size() == toCheck.size())
			return i;
	}
	throw out_of_range("rule not found");
}
int DependancyGraph::findRule(RefinedRule rule) {
	for (int i = 0; i < ruleIndex.size(); i++) {
		RefinedRule toCheck = ruleIndex.at(i);
		if (rule.isSame(toCheck))
			return i;
	}
	throw out_of_range("rule not found");
}
vector<int> DependancyGraph::findRules(Scheme scheme) {
	vector <int> matches;
	for (int i = 0; i < ruleIndex.size(); i++) {
		Scheme toCheck = ruleIndex.at(i).getHeadScheme();
		if (scheme.getName() == toCheck.getName() && scheme.size() == toCheck.size())
			matches.push_back(i);
	}
	return matches;
}

void DependancyGraph::buildDependancies(const vector<RefinedRule> &rules) {
	for (RefinedRule rule : rules) {
		Scheme conclusion = rule.getHeadScheme();
		vector<Scheme> premises = rule.getSchemeList();
		for (Scheme premise : premises) {
			try {
				int con = findRule(conclusion);
				vector <int> prereqs = findRules(premise);
				for (int prereq : prereqs) {
					dependancies.push_back(Edge(con, prereq));
				}
			} catch (out_of_range& e) {
				//do nada this just means we dont have a rule named after that scheme
			}
		}
	}
}

void DependancyGraph::buildGraph() {
	for (int i = 0; i < ruleIndex.size(); i++) {
		RefinedRule refinedRule = ruleIndex.at(i);
		RuleNode ruleNode = RuleNode(i, refinedRule);
		graph.emplace(i, ruleNode);
	}
	for (Edge edge : dependancies) {
		try {
			graph.at(edge.getCon()).addPrereq(edge.getPrereq());
		} catch (out_of_range& e) {
			RuleNode ruleNode = RuleNode(edge.getCon(),
					ruleIndex.at(edge.getCon()));
			ruleNode.addPrereq(edge.getPrereq());
			graph.emplace(edge.getCon(), ruleNode);
		}
	}
}
void DependancyGraph::buildReverseGraph() {
	for (Edge edge : dependancies) {
		try {
			reverseGraph.at(edge.getPrereq()).addPrereq(edge.getCon());
		} catch (out_of_range& e) {
			RuleNode ruleNode = RuleNode(edge.getPrereq(),
					ruleIndex.at(edge.getPrereq()));
			ruleNode.addPrereq(edge.getCon());
			reverseGraph.emplace(edge.getPrereq(), ruleNode);
			//note
		}
	}
}

void DependancyGraph::printGraph() {
	*out << "Dependency Graph" << endl;
	for (int i = 0; i < totalRules; i++) {
		try {
			*out << "  " << graph.at(i).toString();
		} catch (out_of_range& e) {
			*out << "CAUGHT... SHOULDN'T HAVE CAUGHT" << endl;
		}
	}
	*out << endl;
}
void DependancyGraph::printReverseGraph() {
	*out << "Reverse Graph" << endl;
	for (int i = 0; i < totalRules; i++) {
		try {
			*out << "  " << reverseGraph.at(i).toString();
		} catch (out_of_range& e) {
			*out << "Might Be able to catch... IDK" << endl;
		}
	}
	*out << endl;
}
void DependancyGraph::printOrder() {
	*out << "Postorder Numbers" << endl;
	queue <int> orderCopy = order;
	map <int, int> ruleToOrder;
	for(int i = 0; orderCopy.size() > 0; i++) {
		int ruleNum = orderCopy.front();
		orderCopy.pop();
		ruleToOrder.emplace(ruleNum, i+1);
	}
	for (int i = 0; i < totalRules; i++) {
		try {
			*out << "  R" << i << ": " << ruleToOrder.at(i) << endl;
		} catch (out_of_range& e) {

		}
	}
	*out << endl;
}
void DependancyGraph::printSCC() {
	*out << "SCC Search Order" << endl;
	deque<RuleTree> copy = results;
	while(copy.size() > 0) {
		RuleTree ruleTree = copy.front();
		for (RefinedRule rule : ruleTree) {
			*out << "  " << findRule(rule) << endl;
		}
		copy.pop_front();
	}
	*out << endl;
}

void DependancyGraph::dfsForestReverse() {
	for (int i = 0; i < ruleIndex.size(); i++) {
		try {
			bool visited = reverseGraph.at(i).isVisited();
			if (!visited) {
				//*out << "NotVisited" << endl;
				dfsReverse(reverseGraph.at(i));
			}
		} catch (out_of_range& e) {
			// do nothing, its fine if we catch,
			//that just means the rule had no dependencies this direction.
			//I think...
		}
	}
}
void DependancyGraph::dfsReverse(RuleNode &ruleNode) {
	ruleNode.setVisited(true);
	//*out << ruleNode.getIndex() << ' ' << ruleNode.getRule().toString() << endl;
	//*out << ruleNode.getPrereqs().size() << endl;
	for (int ruleIndex : ruleNode.getPrereqs()) {
		try {
			//*out << "  " << ruleIndex << ' ' << reverseGraph.at(ruleIndex).getRule().toString() << endl;
			RuleNode nextNode = reverseGraph.at(ruleIndex);
			if (!nextNode.isVisited()) {
				dfsReverse(nextNode);
			}
		} catch (out_of_range& e) {
			// do nothing, its fine if we catch,
			//that just means the rule had no dependencies this direction.
			//I think...
		}
	}
	order.push(ruleNode.getIndex());
}

void DependancyGraph::dfsForest() {
	while (order.size() > 0) {
		int pos = order.front();
		try {
			bool visited = graph.at(pos).isVisited();
			if (!visited) {
				results.push_back(RuleTree());
				dfs(graph.at(pos));
			}
		} catch (out_of_range& e) {
			// do nothing, its fine if we catch,
			//that just means the rule had no dependencies this direction.
			//I think...
		}
		order.pop();
	}
}
void DependancyGraph::dfs(RuleNode &ruleNode) {
	ruleNode.setVisited(true);
	set <int> prereqs = ruleNode.getPrereqs();
	for (int ruleIndex : prereqs) {
		try {
			RuleNode nextNode = graph.at(ruleIndex);
			if (!nextNode.isVisited()) {
				dfs(nextNode);
			}
		} catch (out_of_range& e) {
			// do nothing, its fine if we catch,
			//that just means the rule had no dependencies this direction.
			//I think...
		}
	}
	RefinedRule rule = ruleNode.getRule();
	results.back().push_back(rule);
}
