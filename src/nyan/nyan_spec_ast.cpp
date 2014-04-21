#include "nyan_spec_ast.h"

namespace nyan {

ASTNyanAttribute::ASTNyanAttribute(Token name, Token type, bool is_set=false)
		:
		name{std::move(name)},
		type{std::move(type)},
		is_set{is_set} {
}

std::string ASTNyanAttribute::to_string() const {
	if (is_set) {
		return name.content + " : set(" + type.content + ")\n";
	} else {
		return name.content + " : " + type.content + "\n";
	}
}

ASTNyanDelta::ASTNyanDelta(Token type)
		:
		type{std::move(type)} {
}

std::string ASTNyanDelta::to_string() const {
	return "^" + type.content + "\n";
}

ASTNyanType::ASTNyanType(Token name)
		:
		name{std::move(name)} {
}

std::string ASTNyanType::to_string() const {
	std::string result;
	result += name.content + " {\n";
	for (auto &attr : attributes) {
		result += "\t" + attr.to_string();
	}
	if (allow_dynamic_attributes) {
		result += "\t...\n";
	}
	for (auto &delta : deltas) {
		result += "\t" + delta.to_string();
	}
	result += "}\n";
	return result;
}

std::string ASTNyanSpec::to_string() const {
	std::string result;
	for (auto &type : types) {
		result += type.to_string() + "\n";
	}
	return result;
}

}
