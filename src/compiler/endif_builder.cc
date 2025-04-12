#include <compiler/compiler.hpp>
#include <log.hpp>

using namespace llvm;

bool Compiler::doEndif(SExprObject object) {
	WofFunction& func = functions[currentFunction];
	if (func.ifStatements.empty()) {
		ERROR("[{}] No if-statements to end", object.children[0].token.line);
		return false;
	}

	WofIfStatement& ifst = func.ifStatements.top();

	// Ensure all blocks branch to merge block if not already terminated
	if (!ifst.trueBlock->getTerminator())
		builder.SetInsertPoint(ifst.trueBlock);
	builder.CreateBr(ifst.mergeBlock);

	if (!ifst.falseBlock->getTerminator()) {
		builder.SetInsertPoint(ifst.falseBlock);
		builder.CreateBr(ifst.mergeBlock);
	}

	// Set insertion point to merge block for following code
	builder.SetInsertPoint(ifst.mergeBlock);

	// Pop the if statement from stack
	func.ifStatements.pop();
	return true;
}
