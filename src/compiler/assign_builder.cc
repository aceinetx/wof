#include <compiler/compiler.hpp>
#include <log.hpp>

using namespace llvm;

bool Compiler::doAssign(SExprObject object) {
	Token &name = object.children[1].token;
	SExprObject &expr = object.children[2];

	if (name.type != Token::IDENTIFIER) {
		ERROR("[{}] Excepted an IDENTIFIER as a name", name.line);
		return false;
	}

	WofVariable &var = functions[currentFunction].variables[name.valueS];

	Value *value = doExpr(expr);
	if (!value) {
		return false;
	}

	builder.CreateStore(value, var.value);

	return true;
}
