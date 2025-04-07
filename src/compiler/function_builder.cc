#include "sexpr.hpp"
#include <compiler/compiler.hpp>
#include <log.hpp>

bool Compiler::do_function(SExprObject object) {
	Token &name = object.children[1].token;
	SExprObject &args = object.children[2];
	SExprObject &body = object.children[3];

	if (name.type != Token::IDENTIFIER) {
		ERROR("[{}] Excepted a IDENTIFIER after fun", name.line);
		return false;
	}

	return true;
}
