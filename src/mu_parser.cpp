#include "mu_parser.h"

MuParser::MuParser(const std::vector<MuToken>& tokens) {
	m_reader = MuTokenReader(tokens);
}

NodePtr MuParser::atom() {
	if (m_reader.peek().type == MuToken::MuNumber) {
		auto val = m_reader.read();
		return NodePtr(new NumberNode(val.number));
	}
	return expr();
}

NodePtr MuParser::factor() {
	if (m_reader.peek().type == MuToken::MuSub) {
		m_reader.read();
		return NodePtr(new UnOpNode(MuVM::UnOp::Negative, factor()));
	} else if (m_reader.peek().type == MuToken::MuAdd) {
		m_reader.read();
		return NodePtr(new UnOpNode(MuVM::UnOp::Positive, factor()));
	} else if (m_reader.peek().type == MuToken::MuBitNot) {
		m_reader.read();
		return NodePtr(new UnOpNode(MuVM::UnOp::Not, factor()));
	} else if (m_reader.peek().type == MuToken::MuNot) {
		m_reader.read();
		return NodePtr(new UnOpNode(MuVM::UnOp::LogicNot, factor()));
	}
	return atom();
}

NodePtr MuParser::term() {
	auto left = factor();
	while (m_reader.peek().type == MuToken::MuMultiply ||
		m_reader.peek().type == MuToken::MuDivide ||
		m_reader.peek().type == MuToken::MuModulo)
	{
		auto type = m_reader.read().type;
		auto right = term();
		MuVM::BinOp op;
		switch (type) {
			case MuToken::MuMultiply: op = MuVM::BinOp::Mul; break;
			case MuToken::MuDivide: op = MuVM::BinOp::Div; break;
			case MuToken::MuModulo: op = MuVM::BinOp::Mod; break;
			default: return nullptr;
		}
		return NodePtr(new BinOpNode(std::move(left), op, std::move(right)));
	}
	return left;
}

NodePtr MuParser::arith() {
	auto left = term();
	while (m_reader.peek().type == MuToken::MuAdd ||
		m_reader.peek().type == MuToken::MuSub)
	{
		auto type = m_reader.read().type;
		auto right = arith();
		MuVM::BinOp op;
		switch (type) {
			case MuToken::MuAdd: op = MuVM::BinOp::Add; break;
			case MuToken::MuSub: op = MuVM::BinOp::Sub; break;
			default: return nullptr;
		}
		return NodePtr(new BinOpNode(std::move(left), op, std::move(right)));
	}
	return left;
}

NodePtr MuParser::expr() {
	if (m_reader.peek().lexeme == "(") {
		m_reader.read();
		auto exp = arith();
		if (m_reader.peek().lexeme != ")") return nullptr;
		m_reader.read();
		return exp;
	}
	return arith();
}