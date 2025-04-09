#include <compiler/compiler.hpp>
#include <log.hpp>

using namespace llvm;

bool Compiler::doElif(SExprObject object) {
	if (object.children.size() != 3) {
		ERROR("[{}] ELIF should have 3 operands (has {})", object.line, object.children.size());
		return false;
	}

	WofFunction &func = functions[currentFunction];
	if (func.ifStatements.empty()) {
		ERROR("[{}] No if-statements to elif", object.children[0].token.line);
		return false;
	}

	SExprObject comparison = object.children[1];
	SExprObject block = object.children[2];

	Value *comparisonValue = doExpr(comparison);
	WofIfStatement &parentIfst = func.ifStatements.top();

	unsigned int id = ++ifID;
	BasicBlock *trueBlock = BasicBlock::Create(context, std::format("{}t", id), func.function);
	BasicBlock *falseBlock = BasicBlock::Create(context, std::format("{}f", id), func.function);

	builder.SetInsertPoint(parentIfst.falseBlock);
	builder.CreateCondBr(comparisonValue, trueBlock, falseBlock);

	builder.SetInsertPoint(trueBlock);
	if (!doBlock(block))
		return false;

	if (!builder.GetInsertBlock()->getTerminator())
		builder.CreateBr(parentIfst.mergeBlock);

	parentIfst.falseBlock = falseBlock;
	parentIfst.elif = true;

	builder.SetInsertPoint(falseBlock);

	return true;
}
