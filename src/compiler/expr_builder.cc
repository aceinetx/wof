#include <compiler/compiler.hpp>
#include <log.hpp>

using namespace llvm;

Value *Compiler::doExpr(SExprObject object) {
	if (object.children.size() == 0) {
		Type *type;
		Constant *cnst;
		if (object.token.type == Token::INTEGER) {
			type = builder.getInt64Ty();
			Constant *cnst = ConstantInt::get(type, object.token.valueI);
			return cnst;
		} else if (object.token.type == Token::FLOATING) {
			type = builder.getDoubleTy();
			Constant *cnst = ConstantInt::get(type, object.token.valueF);
			return cnst;
		} else {
			ERROR("[{}] Invalid one-constant value type", object.token.line);
			return nullptr;
		}
	} else {
		if (object.children.size() == 3) {
			if (object.children[0].token.type == Token::OPERATOR) {
				Value *left = doExpr(object.children[1]);
				Value *right = doExpr(object.children[2]);
				Value *res = nullptr;

				switch (object.children[0].token.valueC) {
				case '+':
					res = builder.CreateAdd(left, right);
					break;
				case '-':
					res = builder.CreateSub(left, right);
					break;
				case '*':
					res = builder.CreateMul(left, right);
					break;
				case '/':
					ERROR("[{}] Division is a TODO", object.token.line);
					break;
				}

				return res;

			} else {
				ERROR("[{}] Excepted an operator", object.token.line);
				return nullptr;
			}
		}
	}

	return nullptr;
}
