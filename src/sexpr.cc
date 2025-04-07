#include <sexpr.hpp>

SExpr::SExpr(Lexer &lexer) : lexer(lexer) {
}

SExprObject SExpr::next() {
	Token token = lexer.next();
	while (token.type != Token::LPAREN) {
		token = lexer.next();
		if (token.type == Token::END) {
			SExprObject object;
			object.isNull = true;

			return object;
		}
	}
}