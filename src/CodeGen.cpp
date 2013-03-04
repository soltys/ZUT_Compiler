#include <iostream>
#include <sstream>
#include <stdexcept>
#include "Node.h"
#include "Instruction.h"
#include "CodeGen.h"
namespace PSLang {
std::ostream& operator <<(std::ostream& o, const Instruction& a) {
	o << a.instruction << std::endl;
	return o;
}
template<class T>
std::string toString(T a) {
	std::stringstream out;
	out << a;
	return out.str();
}

void CodeGenContext::generateCode(NBlock &root) {
	std::cout << "Generating code...\n";
	this->outputStream << root.codeGen(*this);
	std::cout << "Code is generated.\n";
}

std::string NInteger::codeGen(CodeGenContext& context) {
	std::stringstream ss;
	ss << Instruction("MOV", "R0", toString(value));
	context.resultRegister = std::string("R0");
	return ss.str();
}

std::string NDouble::codeGen(CodeGenContext& context) {
	std::stringstream ss;
	ss << Instruction("MOV", "F0", toString(value));
	context.resultRegister = "F0";
	return ss.str();
}

std::string NString::codeGen(CodeGenContext& context) {

}

std::string NIdentifier::codeGen(CodeGenContext& context) {

}

std::string NMethodCall::codeGen(CodeGenContext& context) {

}

std::string NBinaryOperator::codeGen(CodeGenContext& context) {

}

std::string NAssignment::codeGen(CodeGenContext& context) {
	std::stringstream ss;
	std::cout << "Creating assignment for " << lhs.name << std::endl;

	if (context.locals.find(lhs.name) == std::end(context.locals)) {
		throw std::runtime_error("Variable is not declared");
	}
	ss << rhs.codeGen(context);

	auto value = Instruction::memoryParam(context.locals[lhs.name]);
	auto instruction = Instruction("MOV", value, context.resultRegister);
	ss << instruction;
	return ss.str();
}

std::string NBlock::codeGen(CodeGenContext& context) {
	std::stringstream ss;
	StatementList::const_iterator it;
	for (it = statements.begin(); it != statements.end(); it++) {
		std::cout << "Generating code for " << typeid(**it).name() << std::endl;
		ss << (**it).codeGen(context);
	}
	return ss.str();
}

std::string NExpressionStatement::codeGen(CodeGenContext& context) {
	std::stringstream ss;
	ss << expression.codeGen(context);
	return ss.str();
}

std::string NVariableDeclaration::codeGen(CodeGenContext& context) {
	std::stringstream ss;
	std::cout << "Creating variable declaration " << type.name << " " << id.name
			<< std::endl;

	if (context.locals.find(id.name) == std::end(context.locals)) {
		int maxMemoryIndex = 0;
		for (auto it = std::begin(context.locals);
				it != std::end(context.locals); it++) {
			maxMemoryIndex = std::max(maxMemoryIndex, it->second);
		}

		context.locals.insert(std::make_pair(id.name, ++maxMemoryIndex));
	}

	if (assignmentExpression != nullptr) {
		NAssignment assn(id, *assignmentExpression);
		ss << assn.codeGen(context);
	}
	return ss.str();
}

std::string NFunctionDeclaration::codeGen(CodeGenContext& context) {

}

}
