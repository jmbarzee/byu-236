#include <string>
#include <sstream>

#include "Token.h"

using namespace std;

string Token::toString() {
	stringstream ss;
	ss << '(' << name << ",\"" << literal << "\"," << line << ')' << '\n';
	return ss.str();
}
bool Token::isType(string name) {
	if (name == this->name)
		return true;
	return false;
}
