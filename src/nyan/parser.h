#ifndef _NYAN_PARSER_H_
#define _NYAN_PARSER_H_

#include <stdexcept>
#include <string>
#include <vector>

#include "parser_error.h"
#include "token.h"

namespace nyan {

class Parser {
protected:
	std::vector<Token> tokens;
	std::vector<Token>::iterator token;

public:
	Parser(std::vector<Token> tokens);
	virtual ~Parser() = default;

protected:
	virtual void next_token();

	virtual bool accept_token(Token::type_t type);
	virtual bool accept_token(Token::type_t type,
			const std::string &content);

	virtual bool has_token(Token::type_t type);
	virtual bool has_token(Token::type_t type,
			const std::string &content);
};

}

#endif
