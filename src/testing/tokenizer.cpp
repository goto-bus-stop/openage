#include "tokenizer.h"

#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "../nyan/nyan.h"
#include "../nyan/tokenizer.h"
#include "../nyan/nyan_spec_analyzer.h"
#include "../nyan/nyan_spec_parser.h"
#include "../engine/log.h"
#include "../engine/util/file.h"

using namespace engine;
using namespace nyan;

namespace testing {

int to_real_pos(const std::string &str, int pos) {
	int cnt = 0;
	for (int i = 0; i < pos; i++) {
		if (str[i] == '\t') {
			cnt += 8;
		} else {
			cnt++;
		}
	}
	return cnt;
}

bool tests::tokenizer0(int /*unused*/, char ** /*unused*/) {	
	const char *srcpath = "/tmp/openage.nyanspec";
	std::ifstream fin{srcpath};
	if(!fin) {
		std::cout << "Could not open " << srcpath << std::endl;
		return false;
	}

	std::vector<std::string> lines;
	// read the whole file to the input buffer
	std::ostringstream buffer;
	std::string line;
	while(std::getline(fin, line)) {
		lines.push_back(line);
		buffer << line << '\n';
	}
	std::string input = buffer.str();

	try {
		/*
		auto start = std::chrono::steady_clock::now();
		auto spec = parse_spec(input);
		auto end = std::chrono::steady_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
				end - start);
		std::cout << "\n" << spec->to_string() << std::endl;
		std::cout << "Parsing nyan spec took " << duration.count() << " ms" <<
				std::endl;
		*/
	//	/*
		Tokenizer tokenizer{input};
		auto start = std::chrono::steady_clock::now();
		auto tokens = tokenizer.tokenize();
		auto end = std::chrono::steady_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
				end - start);
		for (auto &token : tokens) {
			log::msg("%3d:%2d: %-20s %s", token.line, token.position,
					Token::get_string(token.type), token.content.c_str());
		}
		std::cout << tokens.size() << " tokens took " << duration.count() <<
				" ms" << std::endl;

		NyanSpecParser parser{std::move(tokens)};
		start = std::chrono::steady_clock::now();
		auto spec_ast = parser.parse();
		end = std::chrono::steady_clock::now();
		duration = std::chrono::duration_cast<std::chrono::milliseconds>(
				end - start);
		std::cout << "\n" << spec_ast->to_string() << std::endl;
		log::msg("parsing spec took %lu ms", duration.count());

		NyanSpecAnalyzer analyzer{std::move(spec_ast)};
		start = std::chrono::steady_clock::now();
		auto spec = analyzer.analyze();
		end = std::chrono::steady_clock::now();
		duration = std::chrono::duration_cast<std::chrono::milliseconds>(
				end - start);
		std::cout << "\n" << spec->to_string() << std::endl;
		log::msg("analyzing spec took %lu ms", duration.count());
	//	*/
	} catch (ParserError &e) {
		auto lin = e.get_line();
		auto pos = e.get_position();
		auto token = e.get_token();
		std::cout << srcpath << ":" << (lin+1) << ":" << (pos+1) <<
				": error: " << e.what() << std::endl;
		auto err_line = lines[lin];
		std::cout << err_line << std::endl;
		auto real_pos = to_real_pos(err_line, pos);
		std::cout << std::string(real_pos, ' ') << "^" << std::endl;
		return false;
	}
	return true;
}

} //namespace testing
