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
protected:
	SymbolType type;
public:

	Symbol(const SymbolType type);
	Symbol();
	virtual std::string getValue() {
		throw std::runtime_error("should not be called never-ever");
		return "";
	}
	SymbolType getType();
	std::string getTypeRegister();
	virtual ~Symbol();
};

class Variable: public Symbol {
protected:

public:
	Variable(const int offset, const SymbolType type);
	Variable(const int offset, const int size, const SymbolType type);
	Variable(const int offset, std::vector<long int> indexes, const SymbolType type);
	virtual std::string getValue();
	int getSize();
	virtual bool isTemporaryValue();
	std::vector<long int> indexes;
	int offset;

};

class TemporaryVariable: public Variable {
public:
	TemporaryVariable(const int offset, const SymbolType type);
	TemporaryVariable(const int offset, const int size, const SymbolType type);
	virtual bool isTemporaryValue();
};

class IntConstant: public Symbol {
	long long value;
public:
	IntConstant(long long value);
	virtual std::string getValue() {
		return toString(value);
	}
	virtual ~IntConstant() {
	}
};

class FloatConstant: public Symbol {
	double value;
public:
	FloatConstant(double value);
	virtual std::string getValue() {
		return toString(value);
	}
	virtual ~FloatConstant() {
	}
};

} /* namespace PSLang */
#endif /* SYMBOL_H */
