/*
 * Parameter.cpp
 *
 *  Created on: May 14, 2015
 *      Author: jacobmb
 */

#include <string>

#include "Parameter.h"
#include "Token.h"

string Parameter::toString() {
	return token.getLiteral();
}


