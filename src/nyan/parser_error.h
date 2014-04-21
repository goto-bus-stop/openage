#ifndef _NYAN_PARSER_ERROR_H_
#define _NYAN_PARSER_ERROR_H_

#include <stdexcept>
#include <string>

namespace nyan {

struct Token;

class ParserError : public std::runtime_error {
private:
	int line;
	int position;
	std::string token;

public:
	explicit ParserError(const std::string &what, int line=0,
			int position=0, const std::string &token="");
	explicit ParserError(const std::string &what, const Token &token);

	int get_line() const;
	int get_position() const;
	std::string get_token() const;
};

}	

#endif
