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
			WofVariable &var = func.variables[object.token.valueS];
			Value *value = builder.CreateLoad(var.type, var.value);
			return value;
		} else if (object.token.type == Token::STRING) {
			Value *value = builder.CreateGlobalString(object.token.valueS);

			return value;
		} else {
			ERROR("[{}] Invalid one-constant value type ({})", object.token.line, (int)object.token.type);
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

				std::string op = object.children[0].token.valueS;
				if (op == "+") {
					res = builder.CreateAdd(left, right);
				} else if (op == "-") {
					res = builder.CreateSub(left, right);
				} else if (op == "*") {
					res = builder.CreateMul(left, right);
				} else if (op == "/") {
					res = builder.CreateSDiv(left, right);
				} else if (op == "==") {
					res = builder.CreateICmpEQ(left, right);
				} else {
					ERROR("[{}] Invalid operator: {}", object.children[0].token.line, op);
				}

				return res;
			}
		}

		if (object.children[0].token.type == Token::IDENTIFIER) {
			Token name = object.children[0].token;
			Function *func;
			if (functions.contains(name.valueS)) {
				func = functions[name.valueS].function;
			} else {
				func = fmodule.getFunction(name.valueS);
			}

			if (!func) {
				ERROR("[{}] Undefined function: {}", name.line, name.valueS);
				return nullptr;
			}

			std::vector<Value *> args = {};
			std::vector<Type *> argTypes = getFunctionParameterTypes(func);

			for (int i = 1; i < object.children.size(); i++) {
				Value *valueRaw = doExpr(object.children[i]);
				if (!valueRaw) {
					return nullptr;
				}

				Value *value = valueRaw;
				if (i <= func->getFunctionType()->getNumParams()) {
					value = castValue(valueRaw, argTypes[i - 1]);
				}
				if (!value)
					return nullptr;

				args.push_back(value);
			}

			return builder.CreateCall(func, args);
		}
	}

	return nullptr;
}
