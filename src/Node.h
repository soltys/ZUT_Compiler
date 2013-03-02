/*
 * Node.h
 *
 *  Created on: Mar 2, 2013
 *      Author: soltys
 */

#ifndef NODE_H_
#define NODE_H_
#include <iostream>
#include <ostream>
#include <vector>
namespace PSLang {
class CodeGenContext;
class NStatement;
class NExpression;
class NVariableDeclaration;

typedef std::vector<NStatement*> StatementList;
typedef std::vector<NExpression*> ExpressionList;
typedef std::vector<NVariableDeclaration*> VariableList;

class Node {
public:
	Node(){}
	virtual ~Node() {
	}
	virtual void codeGen(CodeGenContext& context) =0;

};

class NExpression: public Node {
public:

};

class NStatement: public Node {
public:

};

class NInteger: public NExpression {
public:
	long long value;
	NInteger(long long value) :
			value(value) {
	}
	virtual void codeGen(CodeGenContext& context);
};

class NDouble: public NExpression {
public:
	double value;
	NDouble(double value) :
			value(value) {
	}
	virtual void codeGen(CodeGenContext& context);
};

class NString: public NExpression {
public:
	std::string value;
	NString(const std::string& value) :
			value(value) {
	}
	virtual void codeGen(CodeGenContext& context);
};

class NIdentifier: public NExpression {
public:
	std::string name;
	NIdentifier(const std::string& name) :
			name(name) {
	}
	virtual void codeGen(CodeGenContext& context);
};

class NMethodCall: public NExpression {
public:
	const NIdentifier& id;
	ExpressionList arguments;
	NMethodCall(const NIdentifier& id, ExpressionList& arguments) :
			id(id), arguments(arguments) {
	}

	NMethodCall(const NIdentifier& id) :
			id(id) {
	}
	virtual void codeGen(CodeGenContext& context);
};

class NBinaryOperator: public NExpression {
public:
	NExpression& lhs;
	int op;
	NExpression& rhs;
	NBinaryOperator(NExpression& lhs, int op, NExpression& rhs) :
			lhs(lhs), op(op), rhs(rhs) {
	}
	virtual void codeGen(CodeGenContext& context);
};

class NAssignment:public NExpression{
public:
	NIdentifier& lhs;
	NExpression& rhs;
	NAssignment(NIdentifier& lhs, NExpression& rhs):lhs(lhs),rhs(rhs){}
	virtual void codeGen(CodeGenContext& context);
};

class NBlock:public NExpression{
public:
	StatementList statements;
	NBlock(){}
	virtual void codeGen(CodeGenContext& context);
};

class NExpressionStatement:public NStatement{
public:
	NExpression& expression;
	NExpressionStatement(NExpression& expression):expression(expression){}
	virtual void codeGen(CodeGenContext& context);
};

class NVariableDeclaration: public NStatement{
public:
	const NIdentifier& type;
	NIdentifier& id;
	NExpression* assignmentExpression;
	NVariableDeclaration(const NIdentifier& type, NIdentifier& id):
		type(type), id(id),assignmentExpression(nullptr){}
	NVariableDeclaration(const NIdentifier& type, NIdentifier& id, NExpression* assignmentExpression):
			type(type), id(id),assignmentExpression(assignmentExpression){}
	virtual void codeGen(CodeGenContext& context);

};

class NFunctionDeclaration:public NStatement{
public:
	const NIdentifier& type;
	const NIdentifier& id;
	VariableList arguments;
	NBlock& block;
	NFunctionDeclaration(const NIdentifier& type, const NIdentifier& id, const VariableList&arguments, NBlock& block):
		type(type), id(id), arguments(arguments),block(block){}
	virtual void codeGen(CodeGenContext& context);
};

} // END NAMESPACE PSLang

#endif /* NODE_H_ */
