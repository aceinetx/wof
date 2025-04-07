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
}
