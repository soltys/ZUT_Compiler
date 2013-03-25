#ifndef SYMBOL_H_
#define SYMBOL_H_

namespace PSLang {
enum SymbolType {
	None, Int, Float
};

class Symbol {
public:
	SymbolType type;
	Symbol(const SymbolType type);
	virtual ~Symbol();
};

class Variable: public Symbol {
public:
	Variable(const int memoryIndex, const SymbolType type);

	int memoryOffset;
	int size;
};

template<class T>
class Constant: public Symbol{
	T value;
public:
	Constant(T value);
};

} /* namespace PSLang */
#endif /* SYMBOL_H */
