#include <iostream>

#include "mu_tokenizer.h"

int main(int argc, char** argv) {
	std::string test = R"(
		let a = 10;
		let b = 3;
		
		print(a + b);
	)";

	MuTokenizer tok(test);
	tok.tokenize();

	for (auto& t : tok.tokens()) {
		std::cout << t.str() << " ";
	}
	std::cout << std::endl;

	return 0;
}