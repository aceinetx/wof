#include <compiler/compiler.hpp>
#include <log.hpp>

using namespace llvm;

bool Compiler::doFunction(SExprObject object) {
	Token &type = object.children[1].token;
	Token &name = object.children[2].token;
	SExprObject &args = object.children[3];
	SExprObject &body = object.children[4];

	if (type.type != Token::IDENTIFIER) {
		ERROR("[{}] Excepted a IDENTIFIER after fun", name.line);
		return false;
	}

	if (name.type != Token::IDENTIFIER) {
		ERROR("[{}] Excepted a IDENTIFIER after function name", name.line);
		return false;
	}

	Type *retType = getTypeFromName(type.valueS);
	if (!retType) {
		return false;
	}

	WofFunction function;
	function.type = FunctionType::get(retType, false);
	function.function = Function::Create(function.type, Function::ExternalLinkage, name.valueS, fmodule);
	function.block = BasicBlock::Create(context, name.valueS + "b", function.function);

	currentFunction = name.valueS;

	functions[name.valueS] = function;

	builder.SetInsertPoint(function.block);

	if (!doBody(body)) {
		return false;
	}

	return true;
}
