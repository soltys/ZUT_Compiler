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
	root.codeGen(*this);
	std::cout << "Code is generated.\n";
}

void NInteger::codeGen(CodeGenContext& context) {
	context.outputStream << Instruction("MOV", "R0", toString(value));
	context.resultRegister = std::string("R0");
}

void NDouble::codeGen(CodeGenContext& context) {
	context.outputStream << Instruction("MOV", "F0", toString(value));
	context.resultRegister = "F0";
}

void NString::codeGen(CodeGenContext& context) {

}

void NIdentifier::codeGen(CodeGenContext& context) {

}

void NMethodCall::codeGen(CodeGenContext& context) {

}

void NBinaryOperator::codeGen(CodeGenContext& context) {

}

void NAssignment::codeGen(CodeGenContext& context) {
	std::cout << "Creating assignment for " << lhs.name << std::endl;

	if (context.locals.find(lhs.name) == std::end(context.locals)) {
		throw std::runtime_error("Variable is not declared");
	}
	rhs.codeGen(context);

	auto value = Instruction::memoryParam(context.locals[lhs.name]);
	auto instruction = Instruction("MOV", value, context.resultRegister);
	context.outputStream << instruction;
}

void NBlock::codeGen(CodeGenContext& context) {
	StatementList::const_iterator it;
	for (it = statements.begin(); it != statements.end(); it++) {
		std::cout << "Generating code for " << typeid(**it).name() << std::endl;
		(**it).codeGen(context);
	}

}

void NExpressionStatement::codeGen(CodeGenContext& context) {
	expression.codeGen(context);
}

void NVariableDeclaration::codeGen(CodeGenContext& context) {
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
		assn.codeGen(context);
	}
}

void NFunctionDeclaration::codeGen(CodeGenContext& context) {

}

}
