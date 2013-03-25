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
Symbol::Symbol() :
		type(None) {

}
Symbol::~Symbol() {

}

Variable::Variable(const int offset, const SymbolType type) :
		Symbol(type), offset(offset), size(1) {
}
Variable::Variable(const int offset, const int size, const SymbolType type) :
		Symbol(type), offset(offset), size(size) {
}

template<class T>
Constant<T>::Constant(T value, const SymbolType type) :
		Symbol(type),value(value) {
}

template<class T>
std::string Constant<T>::getValue() {
	return toString(value);
}

std::string Variable::getValue() {
	return "#" + toString(offset);
}

} /* namespace PSLang */
