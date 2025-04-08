#include <compiler/compiler.hpp>
#include <log.hpp>

using namespace llvm;

bool Compiler::doBlock(SExprObject object) {
	for (SExprObject &obj : object.children) {
		if (obj.children.size() > 0) {
			if (obj.children[0].token.valueS == "return") {
				if (!doReturn(obj))
					return false;
			}
		}
	}

	return true;
}
