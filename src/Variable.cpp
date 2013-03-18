/*
 * Variable.cpp
 *
 *  Created on: Mar 11, 2013
 *      Author: soltys
 */
#include <string.h>
#include "Variable.h"

namespace PSLang {

Variable::Variable(const int memoryOffset, const VariableType type):
		memoryOffset(memoryOffset),
		size(1),
		type(type) {
}

Variable::~Variable() {
}

} /* namespace PSLang */
