#include "nyan.h"

namespace nyan {

std::unique_ptr<NyanSpec> parse_spec(const std::string &input) {
	Tokenizer tokenizer{input};
	auto tokens = tokenizer.tokenize();

	NyanSpecParser parser{std::move(tokens)};
	auto spec_ast = parser.parse();

	NyanSpecAnalyzer analyzer{std::move(spec_ast)};
	auto spec = analyzer.analyze();

	return std::move(spec);
}

}
