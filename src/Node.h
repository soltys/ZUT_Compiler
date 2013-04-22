/*
 * Node.h
 *
 *  Created on: Mar 2, 2013
 *      Author: soltys
 */

#ifndef NODE_H_
#define NODE_H_
#include <iostream>
#include <sstream>
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
typedef std::vector<long int> IndexList;
enum ValueType {
	IntType, DoubleType
};
class Node {
public:
	Node() {
	}
	virtual ~Node() {
	}
	virtual void accept(CodeGenContext& context) =0;

};

class NExpression: public Node {
public:
	virtual void accept(CodeGenContext& context) =0;
};

class NStatement: public Node {
public:
	virtual void accept(CodeGenContext& context) =0;
};


class NInteger: public NExpression {
public:
	long long value;
	NInteger(long long value) :
			value(value) {
	}

	virtual void accept(CodeGenContext& context);

};

class NDouble: public NExpression {
public:
	double value;
	NDouble(double value) :
			value(value) {
	}
	virtual void accept(CodeGenContext& context);

};

class NIdentifier: public NExpression {
public:
	std::string name;
	NIdentifier(const std::string & name) :
			name(name) {
	}
	virtual void accept(CodeGenContext& context);
};

class NArrayIdentifier: public NIdentifier {
public:

	ExpressionList indexes;
	NArrayIdentifier(NIdentifier& id, const ExpressionList& indexes) :
		NIdentifier(id.name),indexes(indexes) {
	}
	virtual void accept(CodeGenContext& context);
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
	virtual void accept(CodeGenContext& context);
};



class NBinaryOperator: public NExpression {
public:
	NExpression& lhs;
	int op;
	NExpression& rhs;
	NBinaryOperator(NExpression& lhs, int op, NExpression& rhs) :
			lhs(lhs), op(op), rhs(rhs) {
	}
	virtual void accept(CodeGenContext& context);
};

class NBooleanOperator: public NBinaryOperator {
public:

	NBooleanOperator(NExpression& lhs, int op, NExpression& rhs) :
		NBinaryOperator(lhs, op, rhs) {
	}
	virtual void accept(CodeGenContext& context);

	void operatorAnd(CodeGenContext& context);
	void operatorOr(CodeGenContext& context);
};

class NAssignment: public NExpression {
public:
	NIdentifier& lhs;
	NExpression& rhs;
	NAssignment(NIdentifier& lhs, NExpression& rhs) :
			lhs(lhs), rhs(rhs) {
	}
	virtual void accept(CodeGenContext& context);
};


class NBlock: public NExpression {
public:
	StatementList statements;
	NBlock() {
	}
	virtual void accept(CodeGenContext& context);
};

class NExpressionStatement: public NStatement {
public:
	NExpression& expression;
	NExpressionStatement(NExpression& expression) :
			expression(expression) {
	}
	virtual void accept(CodeGenContext& context);
};

class NVariableDeclaration: public NStatement {
public:
	const NIdentifier& type;
	NIdentifier& id;
	NExpression* assignmentExpression;
	NVariableDeclaration(const NIdentifier& type, NIdentifier& id) :
			type(type), id(id), assignmentExpression(nullptr) {
	}
	NVariableDeclaration(const NIdentifier& type, NIdentifier& id,
			NExpression* assignmentExpression) :
			type(type), id(id), assignmentExpression(assignmentExpression) {
	}
	virtual void accept(CodeGenContext& context);

};

class NArrayDeclaration: public NVariableDeclaration
{
public:

	IndexList indexes;
	NArrayDeclaration(const NIdentifier& type, NIdentifier& id, const IndexList& indexes):
		NVariableDeclaration(type,id),indexes(indexes){

	}
	virtual void accept(CodeGenContext& context);
};

class NFunctionDeclaration: public NStatement {
public:
	const NIdentifier& type;
	const NIdentifier& id;
	VariableList arguments;
	NBlock& block;
	NFunctionDeclaration(const NIdentifier& type, const NIdentifier& id,
			const VariableList&arguments, NBlock& block) :
			type(type), id(id), arguments(arguments), block(block) {
	}
	virtual void accept(CodeGenContext& context);
};

class NIfStatement:public NStatement{
protected:
	NExpression& boolExpr;
	NBlock& block;
public:
	NIfStatement(NExpression& boolExpr, NBlock& block):
		boolExpr(boolExpr),block(block){}

	virtual void accept(CodeGenContext& context);
};


class NIfElseStatement:public NIfStatement{
protected:
	NBlock& elseBlock;
public:
	NIfElseStatement(NExpression& boolExpr, NBlock& block, NBlock& elseBlock):
		NIfStatement(boolExpr,block),elseBlock(elseBlock){}

	virtual void accept(CodeGenContext& context);
};

class NWhileStatement:public NStatement{
	NExpression& boolExpr;
	NBlock& block;
public:
	NWhileStatement(NExpression& boolExpr, NBlock& block):
		boolExpr(boolExpr),block(block){}

	virtual void accept(CodeGenContext& context);
};


class NForStatement:public NStatement{
	NExpression& assigment;
	NExpression& boolExpr;
	NStatement& exprStmt;
	NBlock& block;
public:
	NForStatement(NExpression& assigment,NExpression& boolExpr,NStatement& exprStmt,  NBlock& block):
		assigment(assigment),boolExpr(boolExpr),exprStmt(exprStmt),block(block){}

	virtual void accept(CodeGenContext& context);
};

} // END NAMESPACE PSLang

#endif /* NODE_H_ */
