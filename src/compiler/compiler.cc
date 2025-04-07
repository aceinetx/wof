#include <compiler/compiler.hpp>

Compiler::Compiler(std::string moduleName, Lexer &lexer) : builder(context), fmodule(moduleName, context), sexpr(lexer) {
}

bool Compiler::compile() {
	SExprObject object = sexpr.next();
	while (object.token.type != Token::END) {
		if (object.children.size() == 4) {
			if (object.children[0].token.valueS == "fun") {
				if (!do_function(object))
					return false;
			}
		}
		object = sexpr.next();
	}

	return true;
}
