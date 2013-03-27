/*
 * Variable.cpp
 *
 *  Created on: Mar 11, 2013
 *      Author: soltys
 */
#include <string>
#include "Symbol.h"
#include "Utils.hpp"
namespace PSLang {
Symbol::Symbol(const SymbolType type) :
		type(type) {

}

Symbol::~Symbol() {

}

Variable::Variable(const int offset, const SymbolType type) :
		Symbol(type), offset(offset), size(1) {
}
Variable::Variable(const int offset, const int size, const SymbolType type) :
		Symbol(type), offset(offset), size(size) {
}
bool Variable::isTemporaryValue()
{
	return false;
}

TemporaryVariable::TemporaryVariable(int offset, SymbolType type):Variable(offset,type){}
TemporaryVariable::TemporaryVariable(int offset, int size, SymbolType type):Variable(offset,size,type){}
bool TemporaryVariable::isTemporaryValue(){
	return true;
}



IntConstant::IntConstant(long long value) :
		Symbol(Int),value(value) {
}
FloatConstant::FloatConstant(double value):
		Symbol(Float), value(value){}


std::string Variable::getValue() {
	return "#" + toString(offset);
}

} /* namespace PSLang */
