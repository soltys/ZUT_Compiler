/*
 * CodeGen.h
 *
 *  Created on: Mar 2, 2013
 *      Author: soltys
 */

#ifndef CODEGEN_H_
#define CODEGEN_H_
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
class CodeGenContext {

	int _temporaryVariableCounter;
	std::map<std::string, int> labels;
public:
	std::ofstream outputStream;
	CodeGenContext() :
			_temporaryVariableCounter(0), outputStream("app.asm") {

	}
	PSLang::Variable createVariable(std::string& name, PSLang::SymbolType type,
			bool isTemporary = false, int size = 1);
	PSLang::Variable createTemporaryVariable(PSLang::SymbolType type);
	void clearTemporaryVariables();

	std::string addJumpWithLabel(const std::string& command, const int& id);
	void createLabel(const std::string& name, const int& value);
	void clearLabels();
	void swapLabels();

	void generateCode(NBlock& root);

	std::map<std::string, Variable_ptr> locals;

	std::stack<Symbol_ptr> valueStack;
	std::vector<PSLang::Instruction> programInstructions;

	virtual ~CodeGenContext() {

	}
};
}
#endif /* CODEGEN_H_ */
