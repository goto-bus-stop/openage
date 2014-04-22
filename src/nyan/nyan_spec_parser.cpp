#include "nyan_spec_parser.h"

#include <algorithm>
#include <cctype>
#include <iostream> // TODO REMOVE

namespace nyan {

std::vector<std::string> NyanSpecParser::BASIC_TYPES = {
		"bool", "int", "float", "string"
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

bool NyanSpecParser::is_basic_type() {
	if (token->type == Token::type_t::IDENTIFIER) {
		return std::find(std::begin(BASIC_TYPES), std::end(BASIC_TYPES),
				token->content) != std::end(BASIC_TYPES);
	}
	return false;
}

void NyanSpecParser::parse_spec() {
	// as long as there are tokens left
	while (token->type != Token::type_t::END) {
		// we expect a type declaration, that begins with an identifier
		if (has_token(Token::type_t::IDENTIFIER)) {
			parse_type_decl();
		} else {
			throw_expected("type identifier");
		}
	}
}

void NyanSpecParser::parse_type_decl() {
	if (is_basic_type()) {
		throw ParserError{"Invalid type identifier: '" + token->content + "'",
				*token};
	}
	// insert new type node into the AST
	ast->types.emplace_back(std::move(*token));
	next_token();

	// a type has to begin with a opening brace
	if (!accept_token(Token::type_t::LBRACE)) {
		throw_expected('{');
	}
	// the parse the type's body
	parse_type_body();
	// and end with a closing brace
	if (!accept_token(Token::type_t::RBRACE)) {
		throw_expected('}');
	}
}

void NyanSpecParser::parse_type_body() {
	// parse all type attributes
	bool got_comma = parse_type_attributes();
	// if the last token was no comma, the type declaration is finished now
	if (got_comma) {
		return;
	}

	// parse '...' to determine whether dynamic attributes are allowed
	bool dynamic_attributes = accept_token(Token::type_t::TRIPLE_DOT);
	ast->types.back().allow_dynamic_attributes = dynamic_attributes;

	if (dynamic_attributes) {
		if (accept_token(Token::type_t::COMMA)) {
			parse_type_deltas(true);
		}
	} else {
		// parse all type deltas
		parse_type_deltas(false);	
	}
}

bool NyanSpecParser::parse_type_attributes() {
	bool got_comma = false;
	while (!got_comma) {
		if (!has_token(Token::type_t::IDENTIFIER)) {
			return false;
		}

		auto &attr_name = *token;
		next_token();

		if (!accept_token(Token::type_t::COLON)) {
			throw_expected(':');
		}

		if (!has_token(Token::type_t::IDENTIFIER)) {
			throw_expected("type");
		}
		auto &attr_type = *token;
		next_token();

		if (attr_type.content == "set") {
			if (!accept_token(Token::type_t::LPAREN)) {
				throw_expected("set type");
			}

			if (!has_token(Token::type_t::IDENTIFIER)) {
				throw_expected("set type");
			}
			auto &set_attr_type = *token;
			next_token();

			if (!accept_token(Token::type_t::RPAREN)) {
				throw_expected(')');
			}

			ast->types.back().attributes.emplace_back(std::move(attr_name),
					std::move(set_attr_type), true);
		} else {
			ast->types.back().attributes.emplace_back(std::move(attr_name),
					std::move(attr_type), false);
		}

		got_comma = !accept_token(Token::type_t::COMMA);
	}

	return got_comma;
}

void NyanSpecParser::parse_type_deltas(bool must) {
	if (!has_token(Token::type_t::CIRCUM)) {
		if (must) {
			throw_expected("delta type");
		} else {
			return;
		}
	}
	while (true) {
		if (!accept_token(Token::type_t::CIRCUM)) {
			throw_expected("delta type");
		}

		if (!has_token(Token::type_t::IDENTIFIER)) {
			throw_expected("type identifier");
		}
		ast->types.back().deltas.emplace_back(std::move(*token));
		next_token();

		if (!accept_token(Token::type_t::COMMA)) {
			break;
		}
	}
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
