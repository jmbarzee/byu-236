/*
 * Edge.h
 *
 *  Created on: Aug 6, 2015
 *      Author: jacobmb
 */

#ifndef EDGE_H_
#define EDGE_H_

using namespace std;

class Edge {
private:
	int con;
	int prereq;
public:
	Edge(int con, int prereq) {
		this->con = con;
		this->prereq = prereq;
	}
	int getCon() {
		return con;
	}
	int getPrereq() {
		return prereq;
	}
};

#endif /* EDGE_H_ */
