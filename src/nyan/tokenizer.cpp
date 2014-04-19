#include "tokenizer.h"

#include <cctype>

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
	case '\n':
		return type_t::NEWLINE;
	default:
		throw TokenizerError{std::string{"Unexpected character: '"} + c + "'"};
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
	case type_t::NEWLINE:
		return "NEWLINE";
	case type_t::END:
		return "END";
	default:
		throw TokenizerError{"No valid token type"};
	}
}

TokenizerError::TokenizerError(const std::string &what, int line, int position)
		:
		std::runtime_error{what},
		line{line},
		position{position} {
}

int TokenizerError::get_line() const {
	return line;
}

int TokenizerError::get_position() const {
	return position;
}

Tokenizer::Tokenizer(const std::string &input)
		: 
		current{input.c_str()},
		last{this->current + input.size()},
		state{state_t::START},
		line{0},
		position{0},
		is_escape{false},
		token_begin{0},
		token_position{0},
		token_line{0},
		token_length{0} {
}

std::vector<Token> Tokenizer::tokenize() {
	char c;

	while (current != last) {
		c = *current;
		process(c);

		if (c == '\n') {
			line++;
			position = 0;
		} else {
			position++;
		}
		current++;
	}
	process(0);
	if (state != state_t::FINISH) {
		throw_unexpected_eof();
	}
	return std::move(tokens);
}

void Tokenizer::begin_token(bool next) {
	token_begin = current + (next ? 1 : 0);
	token_position = position + (next ? 1 : 0);
	// this works because you will not begin a new token at a '\n'
	token_line = line;
	token_length = next ? 0 : 1;
}

void Tokenizer::continue_token() {
	token_length++;
}

void Tokenizer::finish_token(Token::type_t type) {
	tokens.emplace_back(type, token_begin, token_length, line,
			token_position);
}

void Tokenizer::add_token(Token::type_t type) {
	tokens.emplace_back(type, current, 1, line, position);
}

bool Tokenizer::is_single_operator(char c) {
	return c == '=' || c == '^' || c == '!' || c == '@' || c == ':';
}

bool Tokenizer::can_be_dual_operator(char c) {
	// +, - must be handled specially
	return c == '*' || c == '/' || c == '%';
}

bool Tokenizer::is_separator(char c) {
	return c == '(' || c == ')' || c == '[' || c == ']' || c == '{' ||
			c == '}' || c == ',';
}

bool Tokenizer::is_ident_begin(char c) {
	return std::isalpha(c) || c == '_';
}

bool Tokenizer::is_ident(char c) {
	return std::isalnum(c) || c == '_';
}

void Tokenizer::process(char c) {
	switch (state) {
	case state_t::START:
		if (c == '#') {
			state = state_t::COMMENT;
		} else if (std::isspace(c)) {
			state = state_t::START;
		} else if (c == '.') {
			begin_token();
			state = state_t::DOT;
		} else if (is_separator(c)) {
			add_token(Token::get_type_for(c));
			state = state_t::START;
		} else if (is_ident_begin(c)) {
			begin_token();
			state = state_t::IDENTIFIER;
		} else if (c == '0') {
			begin_token();
			state = state_t::ZERO;
		} else if (std::isdigit(c)) { // and not zero
			begin_token();
			state = state_t::INTEGER;
		} else if (c == '+') {
			begin_token();
			state = state_t::PLUS;
		} else if (c == '-') {
			begin_token();
			state = state_t::MINUS;
		} else if (is_single_operator(c)) {
			add_token(Token::get_type_for(c));
			state = state_t::START;
		} else if (can_be_dual_operator(c)) {
			begin_token();
			state = state_t::DUAL_OPERATOR;
		} else if (c == '"') {
			begin_token(true);
			state = state_t::STRING;
		} else if (c == '\'') {
			begin_token(true);
			state = state_t::RAW_STRING;
		} else if (c == 0) {
			state = state_t::FINISH;
		} else {
			throw_unexpected_char(c);
		}
		break;
	case state_t::COMMENT:
		if (c == '\n') {
			state = state_t::START;
		} else if (c == 0) {
			state = state_t::FINISH;
		}
		break;
	case state_t::DOT:
		if (c == '.') {
			continue_token();
			state = state_t::DOT2;
		} else if (std::isdigit(c)) {
			continue_token();
			state = state_t::FLOAT;
		} else if (c == 0) {
			throw_unexpected_eof();	
		} else {
			throw_unexpected_char(c);
		}
		break;
	case state_t::DOT2:
		if (c == '.') {
			continue_token();
			finish_token(Token::type_t::TRIPLE_DOT);
			state = state_t::START;
		} else if (c == 0) {
			throw_unexpected_eof();	
		} else {
			throw_unexpected_char(c);
		}
		break;
	case state_t::PLUS:
		if (c == '=') {
			continue_token();
			finish_token(Token::type_t::ADD);
			state = state_t::START;
		} else if (c == '.') {
			continue_token();
			state = state_t::FLOAT;
		} else if (c == '0') {
			continue_token();
			state = state_t::ZERO;
		} else if (std::isdigit(c)) { // and not zero
			continue_token();
			state = state_t::INTEGER;
		} else {
			finish_token(Token::type_t::PLUS);
			step_back();
		}
		break;
	case state_t::MINUS:
		if (c == '=') {
			continue_token();
			finish_token(Token::type_t::SUB);
			state = state_t::START;
		} else if (c == '.') {
			continue_token();
			state = state_t::FLOAT;
		} else if (c == '0') {
			continue_token();
			state = state_t::ZERO;
		} else if (std::isdigit(c)) { // and not zero
			continue_token();
			state = state_t::INTEGER;
		} else {
			finish_token(Token::type_t::MINUS);
			step_back();
		}
		break;
	case state_t::DUAL_OPERATOR:
		if (c == '=') {
			continue_token();
			if (*token_begin == '*') {
				finish_token(Token::type_t::MUL);
			} else if (*token_begin == '/') {
				finish_token(Token::type_t::DIV);
			} else if (*token_begin == '%') {
				finish_token(Token::type_t::MOD);
			}
			state = state_t::START;
		} else {
			throw_unexpected_char(c);
		}
		break;
	case state_t::IDENTIFIER:
		if (is_ident(c)) {
			continue_token();
		} else {
			finish_token(Token::type_t::IDENTIFIER);
			step_back();
		}
		break;
	case state_t::ZERO:
		if (c == '.') {
			continue_token();
			state = state_t::FLOAT;
		} else if (is_separator(c) || std::isspace(c) || c == 0) {
			finish_token(Token::type_t::INTEGER);
			step_back();
		} else {
			throw_unexpected_char(c);
		}
		break;
	case state_t::INTEGER:
		if (std::isdigit(c)) {
			continue_token();
		} else if (c == '.') {
			continue_token();
			state = state_t::FLOAT;
		} else if (is_separator(c) || std::isspace(c) || c == 0) {
			finish_token(Token::type_t::INTEGER);
			step_back();
		} else {
			throw_unexpected_char(c);
		}
		break;
	case state_t::FLOAT:
		if (std::isdigit(c)) {
			continue_token();
		} else if (is_separator(c) || std::isspace(c) || c == 0) {
			finish_token(Token::type_t::FLOAT);
			step_back();
		} else {
			throw_unexpected_char(c);
		}
		break;
	case state_t::STRING:
		if (is_escape) {
			is_escape = false;
			if (c == '\n') {
				throw_unexpected_char(c);
			} else {
				continue_token();
			}
		} else {
			if (c == '"') {
				finish_token(Token::type_t::STRING);
				state = state_t::START;
			} else if (c == '\\') {
				continue_token();
				is_escape = true;
			} else if (c == '\n') {
				throw_unexpected_char(c);
			} else {
				continue_token();
			}
		}
		break;
	case state_t::RAW_STRING:
		if (c == '\'') {
			finish_token(Token::type_t::RAW_STRING);
			state = state_t::START;
		} else if (c == '\n') {
			throw_unexpected_char(c);
		} else {
			continue_token();
		}
		break;
	case state_t::FINISH:
		break;
	}
}

void Tokenizer::step_back() {
	// reuse last character with start state
	if (!std::isspace(*current)) {
		position--;
		current--;
	}
	state = state_t::START;
}

void Tokenizer::throw_unexpected_char(char c) {
	throw TokenizerError{std::string{"Unexpected character: '"} + c + "'",
			line, position};
}

void Tokenizer::throw_unexpected_eof() {
	throw TokenizerError{"Unexpected end of file", line, position};
}

}
