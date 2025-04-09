#include <compiler/compiler.hpp>
#include <log.hpp>

using namespace llvm;

bool Compiler::doExtern(SExprObject object) {
	if (object.children.size() < 3) {
		ERROR("[{}] Extern should have at least 3 operands", object.children[0].token.line);
		return false;
	}
	for (SExprObject &obj : object.children) {
		if (obj.token.type != Token::IDENTIFIER) {
			ERROR("[{}] Everything in extern should be an identifier", object.children[0].token.line);
			return false;
		}
	}

	Type *retType = getTypeFromName(object.children[1].token.valueS);
	if (!retType)
		return false;
	Token name = object.children[2].token;
	std::vector<Type *> argTypes;
	bool vararg = false;

	for (int i = 3; i < object.children.size(); i++) {
		if (object.children[i].token.valueS == "vararg") {
			vararg = true;
			break;
		}

		Type *type = getTypeFromName(object.children[i].token.valueS);
		if (!type)
			return false;
		argTypes.push_back(type);
	}

	FunctionType *params = FunctionType::get(retType, argTypes, vararg);
	fmodule.getOrInsertFunction(name.valueS, params);

	return true;
}
