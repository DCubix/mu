#ifndef MU_TOKENIZER_H
#define MU_TOKENIZER_H

#include "mu_reader.h"

static std::string TokenTypeString[] = {
	"Assign",
	"Equals",
	"Compare",
	"Number",
	"String",
	"Identifier",
	"Period",
	"BitNot",
	"Modulo",
	"Divide",
	"Multiply",
	"Add",
	"Sub",
	"SHL",
	"SHR",
	"BitAnd",
	"Xor",
	"BitOr",
	"Not",
	"And",
	"Or",
	"Colon",
	"Question",
	"Range",
	"LParen",
	"RParen",
	"Keyword",
	"Semicolon",
	"Comma"
};

struct MuToken {
	std::string lexeme;
	double number;
	enum MuTokenType {
		MuAssign = 0,
		MuEquals,
		MuCompare,
		MuNumber,
		MuString,
		MuIdentifier,
		MuPeriod,
		MuBitNot,
		MuModulo,
		MuDivide,
		MuMultiply,
		MuAdd,
		MuSub,
		MuSHL,
		MuSHR,
		MuBitAnd,
		MuXor,
		MuBitOr,
		MuNot,
		MuAnd,
		MuOr,
		MuColon,
		MuQuestion,
		MuRange,
		MuLParen,
		MuRParen,
		MuKeyword,
		MuSemicolon,
		MuComma,
		MuTypeCount
	} type;

	MuToken() = default;
	MuToken(MuTokenType type, const std::string& lexeme, double value = 0.0)
		: type(type), lexeme(lexeme), number(value) {}
	
	inline std::string str() const {
		return "{" + TokenTypeString[type] + "(" + lexeme + ")}";
	}
};

class MuTokenReader : public MuReader<MuToken> {
public:
	MuTokenReader() = default;
	inline MuTokenReader(const std::vector<MuToken>& tokens) {
		setBuffer(tokens);
	}
};

class MuTokenizer {
public:
	MuTokenizer() = default;
	MuTokenizer(const std::string& source);

	void tokenize();

	const std::vector<MuToken>& tokens() { return m_tokens; }

private:
	std::string m_source;
	std::vector<MuToken> m_tokens;

};

#endif // MU_TOKENIZER_H
