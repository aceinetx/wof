#include <compiler/compiler.hpp>
#include <log.hpp>

using namespace llvm;

bool Compiler::doAssign(SExprObject object) {
	if (object.children.size() < 3) {
		ERROR("[{}] Assign should have at least 3 operands", object.line);
		return false;
	}

	Token& name = object.children[1].token;
	SExprObject& expr = object.children[2];

	if (name.type != Token::IDENTIFIER) {
		ERROR("[{}] Excepted an IDENTIFIER as a name", name.line);
		return false;
	}

	WofVariable* var = getVariable(name.valueS);
	if (!var) {
		return false;
	}

	Value* valueRaw = doExpr(expr);
	if (!valueRaw) {
		return false;
	}

	Value* value = castValue(valueRaw, var->type);
	if (!value) {
		return false;
	}

	builder.CreateStore(value, var->value);

	return true;
}
