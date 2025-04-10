#include <compiler/compiler.hpp>
#include <log.hpp>

using namespace llvm;

bool Compiler::doVar(SExprObject object) {
	if (object.children.size() != 3) {
		ERROR("[{}] Var should have 3 operands", object.line);
		return false;
	}

	Token &type = object.children[1].token;
	Token &name = object.children[2].token;

	if (type.type != Token::IDENTIFIER) {
		ERROR("[{}] Excepted an IDENTIFIER as a type", type.line);
		return false;
	}

	if (name.type != Token::IDENTIFIER) {
		ERROR("[{}] Excepted an IDENTIFIER as a name", type.line);
		return false;
	}

	Type *varType = getTypeFromName(type.valueS);
	if (!varType) {
		return false;
	}

	WofStruct *strukt = nullptr;
	for (auto &[structName, _strukt] : structs) {
		if (_strukt.type == varType) {
			strukt = &_strukt;
		}
	}

	WofVariable var;
	var.value = builder.CreateAlloca(varType, nullptr, name.valueS);
	var.name = name.valueS;
	var.type = varType;
	functions[currentFunction].variables[name.valueS] = var;

	if (strukt) {
		decayStruct(*strukt, var);
	}

	return true;
}
