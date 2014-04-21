#ifndef _NYAN_NYAN_SPEC_PARSER_H_
#define _NYAN_NYAN_SPEC_PARSER_H_

#include "parser.h"
#include "nyan_spec.h"
#include "nyan_spec_ast.h"

#include <memory>
#include <string>
#include <vector>

namespace nyan {

class NyanSpecParser : public Parser {
private:
	std::unique_ptr<ASTNyanSpec> ast;

public:
	static std::vector<std::string> KEYWORDS;

	NyanSpecParser(std::vector<Token> tokens);
	~NyanSpecParser() = default;

	std::unique_ptr<ASTNyanSpec> parse();

private:
	void parse_spec();
	void parse_type_decl();
	void parse_type_body();

	void throw_expected(char exp);
	void throw_expected(const std::string &exp);
};

}

#endif
