#pragma once
#include <lexer.hpp>
#include <llvm/IR/DerivedTypes.h>
#include <pch.hpp>
#include <sexpr.hpp>

typedef struct WofVariable {
	llvm::Value *value;
	llvm::Type *type;
	std::string name;
} WofVariable;

typedef struct WofIfStatement {
	llvm::BasicBlock *trueBlock;
	llvm::BasicBlock *falseBlock;
	llvm::BasicBlock *mergeBlock;
	bool elif;
} WofIfStatement;

typedef struct WofStruct {
	llvm::StructType *type;
	std::vector<llvm::Type *> fieldTypes;
	std::vector<std::string> fieldNames;
	std::vector<std::string> privateFields;
	std::string name;
} WofStruct;

typedef struct WofLoop {
	llvm::BasicBlock *condBlock;
	llvm::BasicBlock *block;
	llvm::BasicBlock *mergeBlock;
	SExprObject condition;
} WofLoop;

typedef struct WofFunction {
	llvm::Function *function;
	llvm::BasicBlock *block;
	llvm::FunctionType *type;

	std::string name;

	std::map<std::string, WofVariable> variables;
	std::stack<WofIfStatement> ifStatements;
} WofFunction;

class Compiler {
public:
	llvm::LLVMContext context;
	llvm::IRBuilder<> builder;
	llvm::Module fmodule;

	SExpr sexpr;

	std::map<std::string, WofFunction> functions;
	std::map<std::string, WofStruct> structs;
	std::map<std::string, llvm::Type *> types;

	unsigned int ifID;
	unsigned int whileID;

	std::string currentFunction;

public:
	Compiler(std::string moduleName, Lexer &lexer);

	llvm::Value *castValue(llvm::Value *value, llvm::Type *targetType);

	void addBasicTypes();
	llvm::Type *getTypeFromName(std::string name);

	bool compile();
	bool emitObject(std::string output);
	bool smartEmit(std::string output);

	bool doFunction(SExprObject object);
	bool doReturn(SExprObject object);
	bool doBlock(SExprObject object);

	bool doVar(SExprObject object);
	bool doAssign(SExprObject object);

	bool doExtern(SExprObject object);

	bool doIf(SExprObject object);
	bool doElse(SExprObject object);
	bool doElif(SExprObject object);
	bool doEndif(SExprObject object);

	bool doWhile(SExprObject object);

	bool doStruct(SExprObject object);
	bool doStructVar(SExprObject object, WofStruct &struc);

	llvm::Value *doExpr(SExprObject);
};
