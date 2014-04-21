#ifndef _NYAN_NYAN_SPEC_AST_H_
#define _NYAN_NYAN_SPEC_AST_H_

#include <vector>

#include "token.h"

namespace nyan {

struct ASTNyanAttribute {
	Token name;
	bool is_set;
	Token type;

	ASTNyanAttribute(Token name, Token type, bool is_set);

	std::string to_string() const;
};

struct ASTNyanDelta {
	Token type;

	ASTNyanDelta(Token type);

	std::string to_string() const;
};

struct ASTNyanType {
	Token name;

	std::vector<ASTNyanAttribute> attributes;
	bool allow_dynamic_attributes;

	std::vector<ASTNyanDelta> deltas;

	ASTNyanType(Token name);

	std::string to_string() const;
};

struct ASTNyanSpec {
	std::vector<ASTNyanType> types;

	std::string to_string() const;
};

}

#endif
