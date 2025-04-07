#pragma once
#include <lexer.hpp>
#include <pch.hpp>

class SExprObject {
public:
	bool isNull;

	std::vector<SExprObject> children;
};

class SExpr {
private:
	Lexer &lexer;

public:
	SExpr(Lexer &lexer);
	SExprObject next();
};