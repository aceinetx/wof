#include <lexer.hpp>
#include <pch.hpp>

int main() {
	std::string code = R"(
(fun main () (
  (123.69)
  (return 0)
))
)";

	Lexer lexer(code);
	Token token = lexer.next();
	while (token.type != Token::END) {
		std::cout << token.type << " " << token.valueS << " " << token.valueI << " " << token.valueF << "\n";
		token = lexer.next();
	}
}
