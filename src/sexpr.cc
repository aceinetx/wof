#include <sexpr.hpp>

SExprObject::SExprObject() {
	token = Token::create();
}

SExpr::SExpr(Lexer &lexer) : lexer(lexer) {
}

SExprObject SExpr::next() {
	SExprObject object;

	Token token = lexer.next();
	while (token.type != Token::LPAREN) {
		token = lexer.next();
		if (token.type == Token::END) {
			object.token.type = Token::END;

			return object;
		}
	}

	token = lexer.next();
	while (token.type != Token::RPAREN) {
		if (token.type == Token::IDENTIFIER || token.type == Token::FLOATING || token.type == Token::INTEGER || token.type == Token::STRING || token.type == Token::OPERATOR) {
			SExprObject obj;

			obj.token = token;
			object.children.push_back(obj);
		} else if (token.type == Token::LPAREN) {
			lexer.pos--;
			SExprObject obj = next();
			obj.token.line = token.line;

			object.children.push_back(obj);
		}
		token = lexer.next();
	}

	return object;
}
