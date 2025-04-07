#include <compiler/compiler.hpp>
#include <log.hpp>

using namespace llvm;

bool Compiler::doReturn(SExprObject object) {
	if (object.children.size() > 1) {
		ERROR("return values is a todo");
		return false;
	}

	builder.CreateRet(nullptr);

	return true;
}
