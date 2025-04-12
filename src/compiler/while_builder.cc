#include <compiler/compiler.hpp>
#include <log.hpp>

using namespace llvm;

bool Compiler::doWhile(SExprObject object) {
	if (object.children.size() != 3) {
		ERROR("[{}] WHILE should have 3 operands (has {})", object.line, object.children.size());
		return false;
	}

	SExprObject comparison = object.children[1];
	SExprObject block = object.children[2];

	WofFunction& func = functions[currentFunction];
	WofLoop loop = {};

	unsigned int id = ++whileID;
	loop.condBlock = BasicBlock::Create(context, std::format("{}wc", id), func.function);
	loop.block = BasicBlock::Create(context, std::format("{}wb", id), func.function);
	loop.mergeBlock = BasicBlock::Create(context, std::format("{}wm", id), func.function);

	builder.CreateBr(loop.condBlock);
	builder.SetInsertPoint(loop.condBlock);

	Value* comparisonValue = doExpr(comparison);
	if (!comparisonValue)
		return false;

	builder.CreateCondBr(comparisonValue, loop.block, loop.mergeBlock);

	builder.SetInsertPoint(loop.block);

	if (!doBlock(block))
		return false;

	builder.CreateBr(loop.condBlock);

	builder.SetInsertPoint(loop.mergeBlock);

	return true;
}
