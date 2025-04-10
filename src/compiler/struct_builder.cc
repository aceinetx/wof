#include <compiler/compiler.hpp>
#include <log.hpp>

using namespace llvm;

bool Compiler::doStructVar(SExprObject object, WofStruct &struc) {
	unsigned int line = object.children[0].token.line;
	if (object.children.size() != 4) {
		ERROR("[{}] VAR in struct should have 4 operands", line);
		return false;
	}

	Token &access = object.children[1].token;
	Token &type = object.children[2].token;
	Token &varName = object.children[3].token;

	if (access.type != Token::IDENTIFIER) {
		ERROR("[{}] struct var's second operand should be an identifier", line);
		return false;
	}

	if (type.type != Token::IDENTIFIER) {
		ERROR("[{}] struct var's third operand should be an identifier", line);
		return false;
	}

	if (varName.type != Token::IDENTIFIER) {
		ERROR("[{}] struct var's fourth operand should be an identifier", line);
		return false;
	}

	Type *llvmVarType = getTypeFromName(type.valueS);
	if (!llvmVarType) {
		ERROR("[{}] no such type for struct var", line);
		return false;
	}

	struc.fieldNames.push_back(varName.valueS);
	struc.fieldTypes.push_back(llvmVarType);

	if (access.valueS != "private" && access.valueS != "public") {
		ERROR("[{}] No such access modifier", line);
		return false;
	}

	if (access.valueS == "private") {
		struc.privateFields.push_back(varName.valueS);
	}

	return true;
}

bool Compiler::doStruct(SExprObject object) {
	if (object.children.size() != 3) {
		ERROR("[{}] STRUCT should have 3 operands (has {})", object.line, object.children.size());
		return false;
	}

	SExprObject name = object.children[1];
	SExprObject block = object.children[2];

	if (name.token.type != Token::IDENTIFIER) {
		ERROR("[{}] structure's name should be an identifier", name.token.line);
		return false;
	}

	if (block.token.type != Token::UNKNOWN) {
		ERROR("[{}] structure's contents should be a s-expression block", name.token.line);
		return false;
	}

	structs[name.token.valueS] = (WofStruct){};
	WofStruct &struc = structs[name.token.valueS];
	struc.name = name.token.valueS;

	for (SExprObject &obj : block.children) {
		if (obj.children.size() > 0) {
			if (obj.children[0].token.valueS == "var") {
				doStructVar(obj, struc);
			}
		}
	}

	struc.type = StructType::create(context, struc.fieldTypes, struc.name);

	return true;
}
