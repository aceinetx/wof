#include <compiler/compiler.hpp>
#include <lexer.hpp>
#include <log.hpp>
#include <pch.hpp>
#include <sexpr.hpp>

int main() {
	std::string code = R"(
(fun i8 main () (
	(return 0)
))
(fun i32 add () (
	(return (* 23 3))
))
)";

	Lexer lexer(code);

	Compiler compiler("wof", lexer);

	bool status = compiler.compile();
	return 0;
}
