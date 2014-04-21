#ifndef _NYAN_NYAN_SPEC_AST_H_
#define _NYAN_NYAN_SPEC_AST_H_

#include <vector>

#include "token.h"

namespace nyan {

struct ASTNyanAttribute {
	Token name;
};

struct ASTNyanDelta {
	Token delta_type;
};

struct ASTNyanType {
	Token name;

	std::vector<ASTNyanAttribute> attributes;
	bool allow_dynamic_attributes;

	std::vector<ASTNyanDelta> deltas;

	ASTNyanType(Token name);
};

struct ASTNyanSpec {
	std::vector<ASTNyanType> types;
};

}

#endif
