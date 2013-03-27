#ifndef SYMBOL_H_
#define SYMBOL_H_
#include "Utils.hpp"
#include <iostream>
#include <stdexcept>
namespace PSLang {
enum SymbolType {
	None, Int, Float
};

class Symbol {
public:
	SymbolType type;
	Symbol(const SymbolType type);
	Symbol();
	virtual std::string getValue(){
		return "";
		}

	virtual ~Symbol();
};

class Variable: public Symbol {
public:
	Variable(const int offset, const SymbolType type);
	Variable(const int offset,const int size, const SymbolType type);
	virtual std::string getValue();
	virtual bool isTemporaryValue();
	int offset;
	int size;
};


class TemporaryVariable : public Variable{
public:
	TemporaryVariable(const int offset, const SymbolType type);
	TemporaryVariable(const int offset,const int size, const SymbolType type);
	virtual bool isTemporaryValue();
};


class IntConstant: public Symbol{
	long long value;
public:
	IntConstant(long long value);
	virtual std::string getValue() {
		return toString(value);
	}
	virtual ~IntConstant(){}
};

class FloatConstant: public Symbol{
	double value;
public:
	FloatConstant(double value);
	virtual std::string getValue(){
		return toString (value);
	}
	virtual ~FloatConstant(){}
};

} /* namespace PSLang */
#endif /* SYMBOL_H */
