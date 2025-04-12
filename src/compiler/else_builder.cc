#include <compiler/compiler.hpp>
#include <log.hpp>

using namespace llvm;

bool Compiler::doElse(SExprObject object) {
	if (object.children.size() != 2) {
		ERROR("[{}] ELSE should have 2 operands (has {})", object.line, object.children.size());
		return false;
	}

	WofFunction& func = functions[currentFunction];
	if (func.ifStatements.empty()) {
		ERROR("[{}] No if-statements to else", object.children[0].token.line);
		return false;
	}

	SExprObject block = object.children[1];
	WofIfStatement& ifst = func.ifStatements.top();

	// Generate else block code in the false block
	builder.SetInsertPoint(ifst.falseBlock);
	if (!doBlock(block))
		return false;

	// Add branch to merge block if not already terminated
	if (!builder.GetInsertBlock()->getTerminator())
		builder.CreateBr(ifst.mergeBlock);

	return true;
}
