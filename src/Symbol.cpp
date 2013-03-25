/*
 * Variable.cpp
 *
 *  Created on: Mar 11, 2013
 *      Author: soltys
 */
#include <string.h>
#include "Symbol.h"

namespace PSLang {
Symbol::Symbol(const SymbolType type) :
		type(type) {

}

Symbol::~Symbol() {

}

Variable::Variable(const int memoryOffset, const SymbolType type) :
		Symbol(type),
		memoryOffset(memoryOffset), size(1)

{
}

} /* namespace PSLang */
