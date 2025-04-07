#pragma once
#include <pch.hpp>

typedef struct Token {
	enum { UNKNOWN, END, LPAREN, RPAREN, IDENTIFIER, STRING, INTEGER, FLOATING } type;
	std::string valueS;
	int valueI;
	double valueF;

	static struct Token create();
} Token;

class Lexer {
private:
	std::string code;

public:
	unsigned int pos;

	Lexer(std::string code);

	static bool isDigit(char c);
	static bool isLetter(char c);
	static bool isIdentifierChar(char c);

	Token number();
	Token identifier();

	Token next();
};
