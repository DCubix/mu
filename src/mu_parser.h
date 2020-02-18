#ifndef MU_PARSER_H
#define MU_PARSER_H

#include <variant>

#include "mu_tokenizer.h"
#include "mu_vm.h"

struct Node {
	virtual ~Node() = default;
	virtual void visit(MuVM& vm) {}
};
using NodePtr = std::unique_ptr<Node>;

struct NumberNode : public Node {
	double value;
	NumberNode() = default;
	NumberNode(double value) : value(value) {}
};

struct UnOpNode : public Node {
	NodePtr right;
	MuVM::UnOp op;

	UnOpNode() = default;
	UnOpNode(MuVM::UnOp op, NodePtr right)
		: op(op), right(std::move(right)) {}
};

struct BinOpNode : public Node {
	NodePtr left, right;
	MuVM::BinOp op;

	BinOpNode() = default;
	BinOpNode(NodePtr left, MuVM::BinOp op, NodePtr right)
		: left(std::move(left)), op(op), right(std::move(right)) {}
};

class MuParser {
public:
	MuParser() = default;
	~MuParser() = default;

	MuParser(const std::vector<MuToken>& tokens);

	NodePtr parse() { return atom(); }

private:
	MuTokenReader m_reader;

	NodePtr atom();
	NodePtr factor();
	NodePtr term();
	NodePtr arith();
	NodePtr expr();
};

#endif // MU_PARSER_H
