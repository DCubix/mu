#include "mu_tokenizer.h"

#include <regex>
#include <algorithm>
#include <sstream>

MuTokenizer::MuTokenizer(const std::string& source) {
	m_source = source;
}

void MuTokenizer::tokenize() {
	MuStringReader sr{ m_source };

	const std::regex rid("[a-zA-Z_]");
	const std::regex rid2("[a-zA-Z_0-9]");
	const std::regex rnum("[a-fA-F0-9\\.xX]");

	const std::vector<std::string> keywords = {
		"let", "return", "break", "continue", "if", "elif", "else", "for", "while", "in", "fun"
	};

	const std::vector<std::string> comp_eq = {
		"==", "!="
	};

	const std::vector<std::string> comp = {
		"<", ">", "<=", ">="
	};

	while (!sr.eof()) {
		char c = sr.peek();
		std::string s(1, c);
		if (std::regex_match(s, rid)) {
			std::string ret = sr.readWhile([&](char c) {
				return std::regex_match(std::string(1, c), rid2);
			});
			if (std::find(keywords.begin(), keywords.end(), ret) != keywords.end()) {
				m_tokens.push_back(MuToken(MuToken::MuKeyword, ret));
			} else {
				m_tokens.push_back(MuToken(MuToken::MuIdentifier, ret));
			}
		} else if (c == '"') {
			sr.read();
			std::string ret = "";
			while (sr.peek() != '"') {
				if (sr.peek() == '\\') {
					char e = sr.read();
					switch (e) {
						case '0': ret += "\0"; break;
						case '\\': ret += "\\"; break;
						case '"': ret += "\""; break;
						case 'r': ret += "\r"; break;
						case 'f': ret += "\f"; break;
						case 'n': ret += "\n"; break;
						case 't': ret += "\t"; break;
						case 'a': ret += "\a"; break;
						case 'v': ret += "\v"; break;
						case 'x': {
							std::string es(1, sr.read());
							es += sr.read();

							std::stringstream ss;
							ss << std::hex << es;
							int code; ss >> code;

							ret += '\x00' + code);
						} break;
						case 'u': {
							std::string es(1, sr.read());
							es += sr.read();
							es += sr.read();
							es += sr.read();

							std::stringstream ss;
							ss << std::hex << es;
							int code; ss >> code;

							ret += '\u0000' + code;
						} break;
						case 'U': {
							std::string es(1, sr.read());
							es += sr.read();
							es += sr.read();
							es += sr.read();
							es += sr.read();
							es += sr.read();
							es += sr.read();
							es += sr.read();

							std::stringstream ss;
							ss << std::hex << es;
							int code; ss >> code;

							ret += '\U00000000' + code;
						} break;
					}
				} else ret += sr.read();
			}
			m_tokens.push_back(MuToken(MuToken::MuIdentifier, ret));
		} else if (std::regex_match(s, rnum)) {
			std::string ret = sr.readWhile([&](char c) {
				return std::regex_match(std::string(1, c), rnum);
			});
			m_tokens.push_back(MuToken(MuToken::MuNumber, ret, std::stod(ret)));
		} else if (c == '<') {
			std::string val(1, sr.read());
			if (sr.peek() == '=') {
				val += sr.read();
				m_tokens.push_back(MuToken(MuToken::MuCompare, val));
			} else if (sr.peek() == '<') {
				val += sr.read();
				if (sr.peek() == '=') {
					val += sr.read();
					m_tokens.push_back(MuToken(MuToken::MuAssign, val));
				} else {
					m_tokens.push_back(MuToken(MuToken::MuSHL, val));
				}
			} else {
				m_tokens.push_back(MuToken(MuToken::MuCompare, val));
			}
		} else if (c == '>') {
			std::string val(1, sr.read());
			if (sr.peek() == '=') {
				val += sr.read();
				m_tokens.push_back(MuToken(MuToken::MuCompare, val));
			} else if (sr.peek() == '>') {
				val += sr.read();
				if (sr.peek() == '=') {
					val += sr.read();
					m_tokens.push_back(MuToken(MuToken::MuAssign, val));
				} else {
					m_tokens.push_back(MuToken(MuToken::MuSHR, val));
				}
			} else {
				m_tokens.push_back(MuToken(MuToken::MuCompare, val));
			}
		} else if (c == '=') {
			std::string val(1, sr.read());
			if (sr.peek() == '=') {
				val += sr.read();
				m_tokens.push_back(MuToken(MuToken::MuEquals, val));
			} else {
				m_tokens.push_back(MuToken(MuToken::MuAssign, val));
			}
		} else if (c == '!') {
			std::string val(1, sr.read());
			if (sr.peek() == '=') {
				val += sr.read();
				m_tokens.push_back(MuToken(MuToken::MuEquals, val));
			} else {
				m_tokens.push_back(MuToken(MuToken::MuNot, val));
			}
		} else if (c == '+') {
			std::string val(1, sr.read());
			if (sr.peek() == '=') {
				val += sr.read();
				m_tokens.push_back(MuToken(MuToken::MuAssign, val));
			} else {
				m_tokens.push_back(MuToken(MuToken::MuAdd, val));
			}
		} else if (c == '-') {
			std::string val(1, sr.read());
			if (sr.peek() == '=') {
				val += sr.read();
				m_tokens.push_back(MuToken(MuToken::MuAssign, val));
			} else {
				m_tokens.push_back(MuToken(MuToken::MuSub, val));
			}
		} else if (c == '*') {
			std::string val(1, sr.read());
			if (sr.peek() == '=') {
				val += sr.read();
				m_tokens.push_back(MuToken(MuToken::MuAssign, val));
			} else {
				m_tokens.push_back(MuToken(MuToken::MuMultiply, val));
			}
		} else if (c == '/') {
			std::string val(1, sr.read());
			if (sr.peek() == '=') {
				val += sr.read();
				m_tokens.push_back(MuToken(MuToken::MuAssign, val));
			} else {
				m_tokens.push_back(MuToken(MuToken::MuDivide, val));
			}
		} else if (c == '%') {
			std::string val(1, sr.read());
			if (sr.peek() == '=') {
				val += sr.read();
				m_tokens.push_back(MuToken(MuToken::MuAssign, val));
			} else {
				m_tokens.push_back(MuToken(MuToken::MuModulo, val));
			}
		} else if (c == '&') {
			std::string val(1, sr.read());
			if (sr.peek() == '=') {
				val += sr.read();
				m_tokens.push_back(MuToken(MuToken::MuAssign, val));
			} else if (sr.peek() == '&') {
				val += sr.read();
				m_tokens.push_back(MuToken(MuToken::MuAnd, val));
			} else {
				m_tokens.push_back(MuToken(MuToken::MuBitAnd, val));
			}
		} else if (c == '|') {
			std::string val(1, sr.read());
			if (sr.peek() == '=') {
				val += sr.read();
				m_tokens.push_back(MuToken(MuToken::MuAssign, val));
			} else if (sr.peek() == '|') {
				val += sr.read();
				m_tokens.push_back(MuToken(MuToken::MuOr, val));
			} else {
				m_tokens.push_back(MuToken(MuToken::MuBitOr, val));
			}
		} else if (c == '^') {
			std::string val(1, sr.read());
			if (sr.peek() == '=') {
				val += sr.read();
				m_tokens.push_back(MuToken(MuToken::MuAssign, val));
			} else {
				m_tokens.push_back(MuToken(MuToken::MuXor, val));
			}
		} else if (c == '[' || c == '{' || c == '(') {
			std::string val(1, sr.read());
			m_tokens.push_back(MuToken(MuToken::MuLParen, val));
		} else if (c == ']' || c == '}' || c == ')') {
			std::string val(1, sr.read());
			m_tokens.push_back(MuToken(MuToken::MuRParen, val));
		} else if (c == ',') {
			sr.read();
			m_tokens.push_back(MuToken(MuToken::MuComma, ""));
		} else if (c == ':') {
			sr.read();
			m_tokens.push_back(MuToken(MuToken::MuColon, ""));
		} else if (c == ';') {
			sr.read();
			m_tokens.push_back(MuToken(MuToken::MuSemicolon, ""));
		} else if (c == '.') {
			sr.read();
			if (sr.peek() == '.') {
				sr.read();
				m_tokens.push_back(MuToken(MuToken::MuRange, ""));
			} else {
				m_tokens.push_back(MuToken(MuToken::MuPeriod, ""));
			}
		} else if (c == '?') {
			sr.read();
			m_tokens.push_back(MuToken(MuToken::MuQuestion, ""));
		} else {
			sr.read();
		}
	}
}
