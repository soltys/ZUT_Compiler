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

std::string Symbol::getTypeRegister() {
	if (type == Int) {
		return "R";
	} else {
		return "F";
	}
}

PSLang::SymbolType Symbol::getType() {
	return type;
}

Variable::Variable(const int offset, const SymbolType type) :
		Symbol(type), offset(offset){
	indexes.push_back(1);
}
Variable::Variable(const int offset, const int size, const SymbolType type) :
		Symbol(type), offset(offset) {
	indexes.push_back(size);
}

Variable::Variable(const int offset, std::vector<long int> indexes, const SymbolType type)
:Symbol(type),indexes(indexes), offset(offset){

}
int Variable::getSize(){
	int size = 1;
	for(auto& index : indexes)
	{
		size *= index;
	}
	return size;
}

bool Variable::isTemporaryValue() {
	return false;
}

TemporaryVariable::TemporaryVariable(int offset, SymbolType type) :
		Variable(offset, type) {
}
TemporaryVariable::TemporaryVariable(int offset, int size, SymbolType type) :
		Variable(offset, size, type) {
}
bool TemporaryVariable::isTemporaryValue() {
	return true;
}

IntConstant::IntConstant(long long value) :
		Symbol(Int), value(value) {
}
FloatConstant::FloatConstant(double value) :
		Symbol(Float), value(value) {
}

std::string Variable::getValue() {
	return "#" + toString(offset);
}

} /* namespace PSLang */
