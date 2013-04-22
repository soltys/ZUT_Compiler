
#ifndef CODEGEN_H_
#define CODEGEN_H_
#include "CodeGenBase.hpp"
#include "Node.h"
#include "Symbol.h"
#include "Instruction.h"
#include <fstream>
#include <stack>
#include <map>
#include <vector>
#include <memory>
namespace PSLang {

class NBlock;
typedef std::shared_ptr<PSLang::Symbol> Symbol_ptr;
typedef std::shared_ptr<PSLang::Variable> Variable_ptr;

/**
 *  Code generating class, based on AST
 */
class CodeGen:public CodeGenBase {

	int _temporaryVariableCounter;
	std::map<std::string, int> labels;
	std::map<std::string, Variable_ptr> locals;
	std::vector<PSLang::Instruction> programInstructions;
	std::stack<Symbol_ptr> valueStack;
public:
	std::ofstream outputStream;
	CodeGen(std::string outputFile) :
			_temporaryVariableCounter(0), outputStream(outputFile) {

	}
	PSLang::Variable createVariable(std::string& name, PSLang::SymbolType type,
			bool isTemporary = false, int size = 1);
	PSLang::Variable createArray(std::string& name, PSLang::SymbolType type,std::vector<long int> indexes);
	PSLang::Variable createTemporaryVariable(PSLang::SymbolType type);
	Variable_ptr getVariable(const std::string& name);
	void clearTemporaryVariables();

	std::string addJumpWithLabel(const std::string& command);
	void createLabel(const std::string& name, const int& value);
	void clearLabels();

	/**
	 * Swaps labels with instruction number based on labels map
	 */
	void swapLabels();

	void createFunction(const std::string& name, int instuctionStart);

	void addValueStackSymbol(Symbol_ptr symbol);
	Symbol_ptr getSymbolFromValueStack();

	void addInstruction(const std::string& cmd, const std::string& param1);
	void addInstruction(const std::string& cmd, const std::string& param1, const std::string& param2);

	void generateCode(NBlock& root);


	std::map<std::string, int> functions;



	virtual ~CodeGen() {

	}
};
}
#endif /* CODEGEN_H_ */
