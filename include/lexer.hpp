#pragma once
#include <pch.hpp>

typedef struct Token {
	enum { UNKNOWN, END, LPAREN, RPAREN, IDENTIFIER, STRING, INTEGER, FLOATING, SEXPR, OPERATOR } type;
	std::string valueS;
	char valueC;
	int valueI;
	double valueF;
	unsigned int line;

	static struct Token create();
} Token;

class Lexer {
private:
	std::string code;

public:
	unsigned int pos;
	unsigned int line;

public:
	Lexer(std::string code);

	static bool isDigit(char c);
	static bool isLetter(char c);
	static bool isIdentifierChar(char c);
	static bool isOperator(char c);

	Token number();
	Token identifier();
	Token str();

	Token next();
};
