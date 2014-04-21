#ifndef _NYAN_TOKEN_H_
#define _NYAN_TOKEN_H_

#include <string>

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

	Token(type_t type, const char *content, int content_length, int line,
			int position);
	~Token() = default;

	static type_t get_type_for(char c);

	static const char *get_string(type_t type);
};

}

#endif
