#ifndef _NYAN_TOKENIZER_H_
#define _NYAN_TOKENIZER_H_

#include <stdexcept>
#include <string>
#include <vector>

namespace nyan {

struct Token {
	enum class type_t {
		// type operators
		PLUS,			// +
		MINUS,			// -
		AT,				// @
		// delta operators
		CIRCUM,			// ^
		EXCLAM,			// !
		// braces
		LPAREN,			// (
		RPAREN,			// )
		LBRACKET,		// [
		RBRACKET,		// ]
		LBRACE,			// {
		RBRACE,			// }
		// dynamic attributes
		TRIPLE_DOT,		// ...
		// value operators
		ASSIGN,			// =
		ADD,			// +=
		SUB,			// -=
		MUL,			// *=
		DIV,			// /=
		MOD,			// %=
		// identifier
		IDENTIFIER,		// [a-zA-Z_] [a-zA-Z0-9_]*
		// literals
		INTEGER,		// integer literal, e.g. 12322
		FLOAT,			// float literal, e.g. 123.324
		STRING,			// "hosd3dg\xdf23hdx"
		RAW_STRING,		// 'asf\\esd23\'
		// misc
		COLON,			// :
		COMMA,			// ,
		NEWLINE,		// \n
		END				// signals the end of token list
	};

	type_t type;
	std::string content;

	int line;
	int position;

	Token(type_t type, std::string content, int line, int position);
	~Token() = default;

	static type_t get_type_for(char c);
	static type_t get_type_for(const std::string &str);

	static const char *get_string(type_t type);
};

class TokenizerError : public std::runtime_error {
private:
	int line;
	int position;

public:
	explicit TokenizerError(const std::string &what, int line=0,
			int position=0);

	int get_line() const;
	int get_position() const;
};

class Tokenizer {
private:
	enum class state_t {
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

	std::string buffer;

	std::vector<Token> tokens;

public:
	Tokenizer(const std::string &input);
	~Tokenizer() = default;

	std::vector<Token> tokenize();

private:
	bool is_single_operator(char c);
	bool can_be_dual_operator(char c);
	bool is_separator(char c);
	bool is_ident_begin(char c);
	bool is_ident(char c);

	void process(char c);
	void step_back();

	void throw_unexpected_char(char c);
	void throw_unexpected_eof();

};

}

#endif
