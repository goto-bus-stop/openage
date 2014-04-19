#include "tokenizer.h"

#include <chrono>
#include <string>

#include "../nyan/tokenizer.h"
#include "../engine/log.h"
#include "../engine/util/file.h"

using namespace engine;
using namespace nyan;

namespace testing {

bool tests::tokenizer0(int /*unused*/, char ** /*unused*/) {	
	char *c_input;
	auto n = util::read_whole_file(&c_input, "/tmp/big.nyan");
	std::string input{c_input};
	delete[] c_input;
	
	Tokenizer tokenizer{input};
	try {
		auto start = std::chrono::steady_clock::now();
		auto tokens = tokenizer.tokenize();
		auto end = std::chrono::steady_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
				end - start);
		for (auto &token : tokens) {
			log::msg("%3d:%2d: %-20s %s", token.line, token.position,
					Token::get_string(token.type), token.content.c_str());
		}
		log::msg("%lu tokens took %lu ms", tokens.size(), duration.count());
	} catch (TokenizerError &e) {
		log::msg("Tokenizer failed at %d:%d: %s", e.get_line()+1,
				e.get_position()+1, e.what());
		return false;
	}
	return true;
}

} //namespace testing
