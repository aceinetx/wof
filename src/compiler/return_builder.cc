#include <compiler/compiler.hpp>
#include <log.hpp>

using namespace llvm;

bool Compiler::doReturn(SExprObject object) {
	if (object.children.size() > 1) {
		Constant *cnst = ConstantInt::get(functions[currentFunction].type->getReturnType(), object.children[1].token.valueI);
		builder.CreateRet(cnst);
	} else {
		builder.CreateRet(nullptr);
	}

	return true;
}
