#include <compiler/compiler.hpp>
#include <log.hpp>

using namespace llvm;

bool Compiler::doBlock(SExprObject object) {
	for (SExprObject &obj : object.children) {
		bool success = false;
		if (obj.children.size() == 3) {
			if (obj.children[0].token.valueS == "var") {
				if (!doVar(obj))
					return false;
				success = true;
			} else if (obj.children[0].token.valueS == "assign") {
				if (!doAssign(obj))
					return false;
				success = true;
			}
		}
		if (obj.children.size() > 0) {
			if (obj.children[0].token.valueS == "return") {
				if (!doReturn(obj))
						return false;
				success = true;
			}
			if(!success){
				if (!doExpr(obj))
						return false;
			}
		}
	}

	return true;
}
