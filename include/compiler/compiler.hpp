#pragma once
#include <lexer.hpp>
#include <pch.hpp>
#include <sexpr.hpp>

class Compiler {
public:
	llvm::LLVMContext context;
	llvm::IRBuilder<> builder;
	llvm::Module fmodule;

	SExpr sexpr;

public:
	Compiler(std::string moduleName, Lexer &lexer);

	bool compile();
	bool do_function(SExprObject object);
};
