#include <lexer.hpp>
#include <util.hpp>

struct Token Token::create() {
	Token token;
	token.type = Token::UNKNOWN;
	token.valueS = "";
	token.valueC = 0;
	token.valueI = 0;
	token.valueF = 0.0f;
	token.line = 0;
	return token;
}

Lexer::Lexer(std::string code) : code(code) {
	pos = 0;
	line = 0;
}

bool Lexer::isDigit(char c) {
	return (c >= '0' && c <= '9');
}

bool Lexer::isLetter(char c) {
	return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
}

bool Lexer::isIdentifierChar(char c) {
	return (isLetter(c) || c == '_');
}

bool Lexer::isOperator(char c) {
	return (c == '+' || c == '-' || c == '*' || c == '/');
}

Token Lexer::number() {
	Token token = Token::create();
	bool is_hex = false;

	unsigned int left_part = 0;
	unsigned int right_part = 0;
	unsigned int right_part_digits = 0;

	token.type = Token::INTEGER;
	token.line = line;

	while (pos < code.length()) {
		char &c = code.at(pos);

		if (c == '.') {
			token.type = Token::FLOATING;
		} else if (isDigit(c)) {
			unsigned int *part = &left_part;
			if (token.type == Token::FLOATING) {
				part = &right_part;
				right_part_digits++;
			}

			*part *= 10;
			*part += c - '0';
		} else {
			break;
		}

		pos++;
	}

	if (token.type == Token::FLOATING) {
		token.valueF = (left_part + (right_part / pow(10, right_part_digits)));
	} else {
		token.valueI = left_part;
	}

	return token;
}

Token Lexer::identifier() {
	Token token = Token::create();

	token.type = Token::IDENTIFIER;
	token.line = line;

	while (pos < code.length()) {
		char &c = code.at(pos);

		if (isIdentifierChar(c) || isDigit(c)) {
			token.valueS.push_back(c);
		} else {
			break;
		}

		pos++;
	}

	return token;
}

Token Lexer::str() {
	Token token = Token::create();

	token.type = Token::STRING;
	token.line = line;

	pos++;
	while (pos < code.length()) {
		char &c = code.at(pos);

		if (c == '"') {
			pos++;
			break;
		} else {
			token.valueS.push_back(c);
		}

		pos++;
	}

	return token;
}

Token Lexer::next() {
	Token token = Token::create();
	while (pos < code.length()) {
		char &c = code.at(pos);
		Token tok = Token::create();

		if (isDigit(c)) {
			token = number();
		} else if (isLetter(c)) {
			token = identifier();
		} else if (c == '"') {
			token = str();
			replace(token.valueS, "\\n", "\n");
			replace(token.valueS, "\\r", "\r");
		} else if (isOperator(c)) {
			token.type = Token::OPERATOR;
			token.valueS = c;
			pos++;
		} else if (c == '=') {
			token.type = Token::OPERATOR;
			token.valueS = c;
			if (pos + 1 < code.size() && code[pos + 1] == '=') {
				token.valueS = "==";
				pos++;
			}
			pos++;
		} else if (c == '(') {
			token.type = Token::LPAREN;
			pos++;
		} else if (c == ')') {
			token.type = Token::RPAREN;
			pos++;
		} else if (c == '\n') {
			line++;
		}

		if (token.type != Token::UNKNOWN) {
			return token;
		}

		pos++;
	}

	token.type = Token::END;
	return token;
}
