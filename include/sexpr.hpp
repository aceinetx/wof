#pragma once
#include <lexer.hpp>
#include <pch.hpp>

class SExprObject {
public:
	Token token;
	std::vector<SExprObject> children;

public:
	SExprObject();
};

class SExpr {
private:
	Lexer &lexer;

public:
	SExpr(Lexer &lexer);
	SExprObject next();
};
