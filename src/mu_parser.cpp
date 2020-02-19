#include "mu_parser.h"

MuParser::MuParser(const std::vector<MuToken>& tokens) {
	m_reader = MuTokenReader(tokens);
}

NodePtr MuParser::expression() {
	if (accepts({ "(" })) {
		eat();
		auto expr = expression();
		if (accepts({ ")" })) {
			eat();
			return std::move(expr);
		}
		return nullptr;
	} else if (accepts({ "+", "-", "~", "!" })) {
		auto tok = eat();
		return NodePtr(new UnOpNode(tok.lexeme, expression()));
	} else if (accepts({ MuToken::MuNumber })) {
		auto left = NodePtr(new NumberNode(eat().number));
		if (accepts({ "|", "&", "^" })) {
			auto tok = eat();
			return NodePtr(new BinOpNode(std::move(left), tok.lexeme, expression()));
		} else if (accepts({ "<<", ">>" })) {
			auto tok = eat();
			return NodePtr(new BinOpNode(std::move(left), tok.lexeme, expression()));
		} else if (accepts({ "*", "/", "%" })) {
			auto tok = eat();
			return NodePtr(new BinOpNode(std::move(left), tok.lexeme, expression()));
		} else if (accepts({ "+", "-" })) {
			auto tok = eat();
			return NodePtr(new BinOpNode(std::move(left), tok.lexeme, expression()));
		} else if (accepts({ "<", ">", "<=", ">=" })) {
			auto tok = eat();
			return NodePtr(new BinOpNode(std::move(left), tok.lexeme, expression()));
		} else if (accepts({ "!=", "==" })) {
			auto tok = eat();
			return NodePtr(new BinOpNode(std::move(left), tok.lexeme, expression()));
		} else if (accepts({ "&&", "||" })) {
			auto tok = eat();
			return NodePtr(new BinOpNode(std::move(left), tok.lexeme, expression()));
		} else if (accepts({ "in", ".." })) {
			auto tok = eat();
			return NodePtr(new BinOpNode(std::move(left), tok.lexeme, expression()));
		}
		return left;
	}

	return nullptr;
}

bool MuParser::accepts(const std::initializer_list<MuToken::MuTokenType>& types) {
	auto tok = m_reader.peek();
	for (auto& t : types) {
		if (tok.type == t) return true;
	}
	return false;
}

bool MuParser::accepts(const std::initializer_list<std::string>& lexemes) {
	auto tok = m_reader.peek();
	for (auto& t : lexemes) {
		if (tok.lexeme == t) return true;
	}
	return false;
}