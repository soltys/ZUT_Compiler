#include <iostream>
#include <sstream>
#include <stdexcept>
#include "Node.h"
#include "Instruction.h"
#include "CodeGen.h"
#include "parser.hh"
#include <algorithm>
namespace PSLang {
typedef PSLang::Parser::token token;
std::ostream& operator <<(std::ostream& o, const Instruction& a) {
	o << a.instruction << std::endl;
	return o;
}

int getIndexFirstFalse(std::vector<bool>& vec) {
	int i = 0;
	for (auto it = std::begin(vec); it != std::end(vec); it++, i++) {
		if (!*it) {
			return i;
		}
	}
	return -1;
}

template<class T>
std::string toString(T a) {
	std::stringstream out;
	out << a;
	return out.str();
}

void CodeGenContext::generateCode(NBlock &root) {
	std::cout << "Generating code...\n";
	root.accept(*this);
	for (auto it = programInstructions.begin();
			it != programInstructions.end();
			it++) {
		outputStream << *it;
	}

	std::cout << "Code is generated.\n";
}
std::string NInteger::getStringValue() {
	std::stringstream ss;
	ss << value;
	return ss.str();
}

ValueType NInteger::getValueType() {
	return ValueType::IntType;
}
void NInteger::accept(CodeGenContext& context) {
	context.valueStack.push(value);
}

void NDouble::accept(CodeGenContext& context) {
	context.valueStack.push(value);
}

void NIdentifier::accept(CodeGenContext& context) {

}

void NMethodCall::accept(CodeGenContext& context) {

}

void NBinaryOperator::accept(CodeGenContext& context) {

	lhs.accept(context);
	rhs.accept(context);

	double lhsValue = context.valueStack.top();
	context.valueStack.pop();

	double rhsValue = context.valueStack.top();
	context.valueStack.pop();

	switch (op) {
	case token::TPLUS:
		context.valueStack.push(lhsValue + rhsValue);
		break;
	case token::TMINUS:
		context.valueStack.push(lhsValue - rhsValue);
		break;
	case token::TMUL:
		context.valueStack.push(lhsValue * rhsValue);
		break;
	case token::TDIV:
		context.valueStack.push(lhsValue / rhsValue);

		break;

	}

}

void NAssignment::accept(CodeGenContext& context) {

	std::cout << "Creating assignment for " << lhs.name << std::endl;

	if (context.locals.find(lhs.name) == std::end(context.locals)) {
		throw std::runtime_error("Variable is not declared");
	}
	rhs.accept(context);

}

void NBlock::accept(CodeGenContext& context) {

	StatementList::const_iterator it;
	for (it = statements.begin(); it != statements.end(); it++) {
		std::cout << "Generating code for " << typeid(**it).name() << std::endl;
		(**it).accept(context);
	}

}

void NExpressionStatement::accept(CodeGenContext& context) {
	expression.accept(context);
}

void NVariableDeclaration::accept(CodeGenContext& context) {
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
		assn.accept(context);
	}

}

void NFunctionDeclaration::accept(CodeGenContext& context) {

}

}
