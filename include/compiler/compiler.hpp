#pragma once
#include <lexer.hpp>
#include <pch.hpp>
#include <sexpr.hpp>

typedef struct WofVariable {
	llvm::Value *value;
	llvm::Type *type;
	std::string name;
} WofVariable;

typedef struct WofFunction {
	llvm::Function *function;
	llvm::BasicBlock *block;
	llvm::FunctionType *type;

	std::string name;

	std::map<std::string, WofVariable> variables;
} WofFunction;

class Compiler {
public:
	llvm::LLVMContext context;
	llvm::IRBuilder<> builder;
	llvm::Module fmodule;

	SExpr sexpr;

	std::map<std::string, WofFunction> functions;
	std::map<std::string, llvm::Type *> types;

	std::string currentFunction;

public:
	Compiler(std::string moduleName, Lexer &lexer);

	llvm::Value *castValue(llvm::Value *value, llvm::Type *targetType);

	void addBasicTypes();
	llvm::Type *getTypeFromName(std::string name);
	bool compile();
	bool doFunction(SExprObject object);
	bool doReturn(SExprObject object);
	bool doBlock(SExprObject object);
	bool doVar(SExprObject object);
	bool doAssign(SExprObject object);
	llvm::Value *doExpr(SExprObject);
};
