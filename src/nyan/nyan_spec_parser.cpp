#include "nyan_spec_parser.h"

#include <algorithm>
#include <cctype>
#include <iostream> // TODO REMOVE

namespace nyan {

std::vector<std::string> NyanSpecParser::KEYWORDS = {
		"bool", "int", "float", "string", "set"
	};

NyanSpecParser::NyanSpecParser(std::vector<Token> tokens)
		:
		Parser{std::move(tokens)} {
}

std::unique_ptr<ASTNyanSpec> NyanSpecParser::parse() {
	ast.reset(new ASTNyanSpec);
	parse_spec();
	return std::move(ast);
}

void NyanSpecParser::parse_spec() {
	while (token->type != Token::type_t::END) {
		if (has_token(Token::type_t::IDENTIFIER)) {
			parse_type_decl();
		} else {
			throw_expected("type identifier");
		}
	}
}

void NyanSpecParser::parse_type_decl() {
	// insert new type
	ast->types.emplace_back(std::move(*token));
	next_token();

	parse_type_body();
}

void NyanSpecParser::parse_type_body() {
}


void NyanSpecParser::throw_expected(char exp) {
	throw ParserError{"Expected '" + std::string{exp} + "', got '" +
			token->content + "'", *token};
}

void NyanSpecParser::throw_expected(const std::string &exp) {
	throw ParserError{"Expected <" + exp + ">, got '" + token->content + "'",
			*token};
}

}
