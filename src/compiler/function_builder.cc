#include <compiler/compiler.hpp>
#include <log.hpp>

using namespace llvm;

bool Compiler::doFunction(SExprObject object) {
	Token &name = object.children[1].token;
	SExprObject &args = object.children[2];
	SExprObject &body = object.children[3];

	if (name.type != Token::IDENTIFIER) {
		ERROR("[{}] Excepted a IDENTIFIER after fun", name.line);
		return false;
	}

	WofFunction function;
	function.type = llvm::FunctionType::get(builder.getVoidTy(), false);
	function.function = Function::Create(function.type, Function::ExternalLinkage, name.valueS, fmodule);
	function.block = BasicBlock::Create(context, name.valueS + "b", function.function);

	builder.SetInsertPoint(function.block);

	for (SExprObject &obj : body.children) {
		if (obj.children.size() > 0) {
			if (obj.children[0].token.valueS == "return") {
				if (!doReturn(obj))
					return false;
			}
		}
	}

	return true;
}
