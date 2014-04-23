#ifndef _NYAN_TOKENIZER_H_
#define _NYAN_TOKENIZER_H_

#include <stdexcept>
#include <string>
#include <vector>

#include "token.h"

namespace nyan {

class Tokenizer {
private:
	/**
	 * The tokenizer's internal state.
	 */
	enum class state_t {
		// the next token can be of any type
		START,
		COMMENT,
		DOT,
		DOT2,
		PLUS,
		MINUS,
		DUAL_OPERATOR,
		IDENTIFIER,
		ZERO,
		INTEGER,
		FLOAT,
		STRING,
		RAW_STRING,
		FINISH
	};

	const char *current;
	const char *last;

	state_t state;

	int line;
	int position;

	bool is_escape;

	const char *token_begin;
	int token_position;
	int token_line;
	int token_length;

	std::vector<Token> tokens;

public:
	Tokenizer(const std::string &input);
	~Tokenizer() = default;

	std::vector<Token> tokenize();

private:
	void begin_token(bool next = false);
	void continue_token();
	void finish_token(Token::type_t type, bool add_current = true);
	void add_token(Token::type_t type);

	bool is_single_operator(char c);
	bool can_be_dual_operator(char c);
	bool is_separator(char c);
	bool is_ident_begin(char c);
	bool is_ident(char c);

	void process(char c);
	void step_back();

	void unexpected_char();
	void unexpected_eof();

};

}

#endif
