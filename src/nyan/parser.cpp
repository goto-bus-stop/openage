#include "parser.h"

#include "util.h"

namespace nyan {

Parser::Parser(std::vector<Token> tokens)
		:
		tokens{std::move(tokens)},
		token{std::begin(this->tokens)} {
}

void Parser::next_token() {
	if (token->type != Token::type_t::END) {
		token++;
	}
}

bool Parser::accept_token(Token::type_t type) {
	if (has_token(type)) {
		next_token();
		return true;
	}
	return false;
}

bool Parser::accept_token(Token::type_t type, const std::string &content) {
	if (has_token(type, content)) {
		next_token();
		return true;
	}
	return false;
}

bool Parser::has_token(Token::type_t type) {
	return token->type == type;
}

bool Parser::has_token(Token::type_t type, const std::string &content) {
	return token->type == type && token->content == content;
}

}
