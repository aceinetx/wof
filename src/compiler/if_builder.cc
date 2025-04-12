#include <compiler/compiler.hpp>
#include <log.hpp>

using namespace llvm;

bool Compiler::doIf(SExprObject object) {
	if (object.children.size() != 3) {
		ERROR("[{}] IF should have 3 operands (has {})", object.line, object.children.size());
		return false;
	}

	SExprObject comparison = object.children[1];
	SExprObject block = object.children[2];

	Value* comparisonValue = doExpr(comparison);
	if (!comparisonValue)
		return false;

	WofFunction& func = functions[currentFunction];

	unsigned int id = ++ifID;
	BasicBlock* trueBlock = BasicBlock::Create(context, std::format("{}t", id), func.function);
	BasicBlock* falseBlock = BasicBlock::Create(context, std::format("{}f", id), func.function);
	BasicBlock* mergeBlock = BasicBlock::Create(context, std::format("{}m", id), func.function);

	builder.CreateCondBr(comparisonValue, trueBlock, falseBlock);

	func.ifStatements.push({});
	WofIfStatement& ifst = func.ifStatements.top();
	ifst.trueBlock = trueBlock;
	ifst.falseBlock = falseBlock;
	ifst.mergeBlock = mergeBlock;
	ifst.elif = false;

	builder.SetInsertPoint(trueBlock);

	if (!doBlock(block))
		return false;

	if (!builder.GetInsertBlock()->getTerminator())
		builder.CreateBr(mergeBlock);

	builder.SetInsertPoint(falseBlock);

	return true;
}
