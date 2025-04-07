#include <lexer.hpp>
#include <pch.hpp>
#include <sexpr.hpp>

int main() {
	std::string code = R"(
(fun main () (
  (return 0)
))
)";

	Lexer lexer(code);
	SExpr sexpr(lexer);

	SExprObject object = sexpr.next();
	return 0;
}
