#include <compiler/compiler.hpp>
#include <lexer.hpp>
#include <log.hpp>
#include <pch.hpp>
#include <sexpr.hpp>

int main() {
	std::string code = R"(
(fun i32 main ((i32 argc)) (
	(var i32 x)
	(var i32 y)
	(var i32 z)

	(assign x 23)
	(assign y 3)
	(assign z (* x y))

	(return z)
))
)";

	Lexer lexer(code);

	Compiler compiler("wof", lexer);

	bool status = compiler.compile();
	return 0;
}
