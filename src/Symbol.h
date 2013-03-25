#ifndef SYMBOL_H_
#define SYMBOL_H_
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
	virtual std::string getValue()=0; //{ throw std::runtime_error("Symbol.getValue() should not be called");}
	virtual ~Symbol();
};

class Variable: public Symbol {
public:
	Variable(const int offset, const SymbolType type);
	Variable(const int offset,const int size, const SymbolType type);
	virtual std::string getValue();
	int offset;
	int size;
};

template<class T>
class Constant: public Symbol{
	T value;
public:
	Constant(T value, const SymbolType type);
	virtual std::string getValue();
};

} /* namespace PSLang */
#endif /* SYMBOL_H */
