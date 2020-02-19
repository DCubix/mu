#ifndef MU_PARSER_H
#define MU_PARSER_H

#include <initializer_list>

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
	std::string op;

	UnOpNode() = default;
	UnOpNode(const std::string& op, NodePtr right)
		: op(op), right(std::move(right)) {}
};

struct BinOpNode : public Node {
	NodePtr left, right;
	std::string op;

	BinOpNode() = default;
	BinOpNode(NodePtr left, const std::string& op, NodePtr right)
		: left(std::move(left)), op(op), right(std::move(right)) {}
};

class MuParser {
public:
	MuParser() = default;
	~MuParser() = default;

	MuParser(const std::vector<MuToken>& tokens);

	NodePtr parse() { return expression(); }

private:
	MuTokenReader m_reader;

	NodePtr expression();

	bool accepts(const std::initializer_list<MuToken::MuTokenType>& types);
	bool accepts(const std::initializer_list<std::string>& lexemes);
	inline MuToken eat() { return m_reader.read(); }
};

#endif // MU_PARSER_H
