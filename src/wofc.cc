#include <compiler/compiler.hpp>
#include <lexer.hpp>
#include <log.hpp>
#include <pch.hpp>
#include <sexpr.hpp>

int main() {
	std::string code = R"(
(fun i32 main ((i32 argc)) (
	(return (- 1 1))
))
)";

	Lexer lexer(code);

	Compiler compiler("wof", lexer);

	bool status = compiler.compile();
	return 0;
}
