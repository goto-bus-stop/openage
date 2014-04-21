#include "parser_error.h"

#include "token.h"
#include "util.h"

namespace nyan {

ParserError::ParserError(const std::string &what, int line, int position,
		const std::string &token)
		:
		std::runtime_error{replace_controls(what)},
		line{line},
		position{position},
		token{token} {
}

ParserError::ParserError(const std::string &what, const Token &token)
		:
		ParserError{what, token.line, token.position, token.content} {
}

int ParserError::get_line() const {
	return line;
}

int ParserError::get_position() const {
	return position;
}

std::string ParserError::get_token() const {
	return token;
}

}
