#include "token.h"

#include "parser_error.h"

namespace nyan {

Token::Token(Token::type_t type, const char *content, int content_length,
		int line, int position)
		:
		type{type},
		content{content, static_cast<std::string::size_type>(content_length)},
		line{line},
		position{position} {
}

Token::type_t Token::get_type_for(char c) {
	switch (c) {
	case '+':
		return type_t::PLUS;
	case '-':
		return type_t::MINUS;
	case '@':
		return type_t::AT;
	case '^':
		return type_t::CIRCUM;
	case '!':
		return type_t::EXCLAM;
	case '(':
		return type_t::LPAREN;
	case ')':
		return type_t::RPAREN;
	case '[':
		return type_t::LBRACKET;
	case ']':
		return type_t::RBRACKET;
	case '{':
		return type_t::LBRACE;
	case '}':
		return type_t::RBRACE;
	case '=':
		return type_t::ASSIGN;
	case ':':
		return type_t::COLON;
	case ',':
		return type_t::COMMA;
	default:
		throw ParserError{std::string{"Unexpected character: '"} + c + "'"};
	}
}

const char *Token::get_string(Token::type_t type) {
	switch (type) {
	case type_t::PLUS:
		return "PLUS";
	case type_t::MINUS:
		return "MINUS";
	case type_t::AT:
		return "AT";
	case type_t::CIRCUM:
		return "CIRCUM";
	case type_t::EXCLAM:
		return "EXCLAM";
	case type_t::LPAREN:
		return "LPAREN";
	case type_t::RPAREN:
		return "RPAREN";
	case type_t::LBRACKET:
		return "LBRACKET";
	case type_t::RBRACKET:
		return "RBRACKET";
	case type_t::LBRACE:
		return "LBRACE";
	case type_t::RBRACE:
		return "RBRACE";
	case type_t::TRIPLE_DOT:
		return "TRIPLE_DOT";
	case type_t::ASSIGN:
		return "ASSIGN";
	case type_t::ADD:
		return "ADD";
	case type_t::SUB:
		return "SUB";
	case type_t::MUL:
		return "MUL";
	case type_t::DIV:
		return "DIV";
	case type_t::MOD:
		return "MOD";
	case type_t::IDENTIFIER:
		return "IDENTIFIER";
	case type_t::INTEGER:
		return "INTEGER";
	case type_t::FLOAT:
		return "FLOAT";
	case type_t::STRING:
		return "STRING";
	case type_t::RAW_STRING:
		return "RAW_STRING";
	case type_t::COLON:
		return "COLOR";
	case type_t::COMMA:
		return "COMMA";
	case type_t::FAIL:
		return "FAIL";
	case type_t::END:
		return "END";
	default:
		throw ParserError{"No valid token type"};
	}
}

}
