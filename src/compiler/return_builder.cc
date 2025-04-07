#include <compiler/compiler.hpp>
#include <log.hpp>

using namespace llvm;

bool Compiler::doReturn(SExprObject object) {
	if (object.children.size() > 1 && object.children.size() <= 2) {
		if (object.children[1].children.size() == 0 && object.children[1].token.type == Token::UNKNOWN) {
			ERROR("[{}] Empty return statement", object.children[0].token.line);
			return false;
		}
		Value *value = doExpr(object.children[1]);
		if (!value) {
			return false;
		}
		Value *castedValue = castValue(value, functions[currentFunction].type->getReturnType());
		if (!castedValue) {
			return false;
		}

		builder.CreateRet(castedValue);
	} else if (object.children.size() == 1) {
		builder.CreateRet(nullptr);
	} else {
		ERROR("[{}] Return statement has an invalid operand count. If you are trying to return an expression, you should wrap it in parens () instead", object.children[0].token.line);
		return false;
	}

	return true;
}
