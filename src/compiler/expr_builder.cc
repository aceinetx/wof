#include <compiler/compiler.hpp>
#include <log.hpp>

using namespace llvm;

std::vector<llvm::Type *> getFunctionParameterTypes(llvm::Function *function) {
	std::vector<llvm::Type *> paramTypes;

	for (auto &arg : function->args()) {
		paramTypes.push_back(arg.getType());
	}

	return paramTypes;
}

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
			Constant *cnst = ConstantFP::get(type, object.token.valueF);
			return cnst;
		} else if (object.token.type == Token::IDENTIFIER) {
			WofFunction &func = functions[currentFunction];
			if (!func.variables.contains(object.token.valueS)) {
				ERROR("[{}] Undefined variable", object.token.line);
				return nullptr;
			}
			return func.variables[object.token.valueS].value;
		} else {
			ERROR("[{}] Invalid one-constant value type", object.token.line);
			return nullptr;
		}
	} else {
		if (object.children.size() == 3) {
			if (object.children[0].token.type == Token::OPERATOR) {
				Value *left = doExpr(object.children[1]);
				if (!left)
					return nullptr;
				Value *rightRaw = doExpr(object.children[2]);
				if (!rightRaw)
					return nullptr;
				Value *right = castValue(rightRaw, left->getType());
				if (!right)
					return nullptr;

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
			} else if (object.children[0].token.type == Token::IDENTIFIER) {
				Token name = object.children[0].token;
				if (!functions.contains(name.valueS)) {
					ERROR("[{}] Undefined function", name.line);
					return nullptr;
				}
				WofFunction &func = functions[name.valueS];

				std::vector<Value *> args = {};
				std::vector<Type *> argTypes = getFunctionParameterTypes(func.function);

				for (int i = 1; i < object.children.size(); i++) {
					Value *valueRaw = doExpr(object.children[i]);
					if (!valueRaw) {
						return nullptr;
					}

					Value *value = castValue(valueRaw, argTypes[i - 1]);
					if (!value)
						return nullptr;

					args.push_back(value);
				}

				return builder.CreateCall(func.function, args);
			} else {
				ERROR("[{}] Excepted an operator or identifier", object.token.line);
				return nullptr;
			}
		}
	}

	return nullptr;
}
