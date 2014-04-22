#ifndef _NYAN_NYAN_SPEC_ANALYZER_H_
#define _NYAN_NYAN_SPEC_ANALYZER_H_

#include <memory>

#include "nyan_spec.h"
#include "nyan_spec_ast.h"

namespace nyan {

class NyanSpecAnalyzer {
private:
	std::unique_ptr<ASTNyanSpec> ast;
	std::unique_ptr<NyanSpec> spec;

public:
	NyanSpecAnalyzer(std::unique_ptr<ASTNyanSpec> ast);
	~NyanSpecAnalyzer() = default;

	std::unique_ptr<NyanSpec> analyze();

private:
	/**
	 * Analyze and create the a NyanSpec from the stored AST.
	 */
	void analyze_spec();
	/**
	 * Create all nyan types, check for duplicated names and set dynamic
	 * attribute flag.
	 */
	void analyze_types();
	void analyze_type_bodies();

	void analyze_deltas(const ASTNyanType &ast_type,
			std::shared_ptr<NyanType> type);
	void analyze_attributes(const ASTNyanType &ast_type,
			std::shared_ptr<NyanType> type);
};

}

#endif
