#include "mu_reader.h"

MuStringReader::MuStringReader(const std::string& data) {
	setBuffer(std::vector<char>(data.begin(), data.end()));
}

std::string MuStringReader::readWhile(const std::function<bool(char)>& func) {
	std::string ret = "";
	while (!eof() && func(peek())) {
		ret += read();
	}
	return ret;
}