#include <iostream>

#include "mu_vm.h"
#include "mu_tokenizer.h"
#include "mu_parser.h"

int main(int argc, char** argv) {
	std::string test = R"(
		(10 + 5) * 2.5
	)";

	MuTokenizer tok(test);
	tok.tokenize();

	for (auto& t : tok.tokens()) {
		std::cout << t.str() << " ";
	}
	std::cout << std::endl;

	MuParser parser(tok.tokens());
	NodePtr res = parser.parse();

	res;

	return 0;
}