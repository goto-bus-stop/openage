#include "nyan_spec_ast.h"

namespace nyan {

ASTNyanType::ASTNyanType(Token name)
		: name{std::move(name)} {
}

}
