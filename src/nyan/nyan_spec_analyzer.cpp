#include "nyan_spec_analyzer.h"

#include "parser_error.h"

namespace nyan {

NyanSpecAnalyzer::NyanSpecAnalyzer(std::unique_ptr<ASTNyanSpec> ast)
		:
		ast{std::move(ast)} {
}

std::unique_ptr<NyanSpec> NyanSpecAnalyzer::analyze() {
	spec.reset(new NyanSpec);
	analyze_spec();
	return std::move(spec);
}

void NyanSpecAnalyzer::analyze_spec() {
	analyze_types();
	analyze_type_bodies();
}

void NyanSpecAnalyzer::analyze_types() {
	for (auto &ast_type : ast->types) {
		auto &type_name = ast_type.name.content;

		// check whether a type with the same name already exists
		auto type_iter = spec->types.find(type_name);
		if (type_iter != std::end(spec->types)) {
			throw ParserError{"Type already exists '" + type_name + "'",
					ast_type.name};
		}

		auto type = std::make_shared<NyanType>(type_name,
				ast_type.allow_dynamic_attributes);
		spec->types.insert({type_name, type});
	}
}

void NyanSpecAnalyzer::analyze_type_bodies() {
	// for each type in the AST
	for (auto &ast_type : ast->types) {
		// fetch the type from the final nyan spec
		auto type = spec->types.find(ast_type.name.content)->second;

		// analyze all attributes of the current type's AST
		analyze_attributes(ast_type, type);
		// analyze all deltas of the current type's AST
		analyze_deltas(ast_type, type);
	}
}

void NyanSpecAnalyzer::analyze_deltas(const ASTNyanType &ast_type,
		std::shared_ptr<NyanType> type) {
	// for each delta in the AST
	for (auto &ast_delta : ast_type.deltas) {
		// determine the delta's type name
		auto &delta_name = ast_delta.type.content;

		// check if the delta's type is defined
		auto type_iter = spec->types.find(delta_name);
		if (type_iter == std::end(spec->types)) {
			throw ParserError{"Unknown type '" + delta_name + "'",
					ast_delta.type};
		}

		// check if a delta with the same name already exists
		auto delta_iter = type->deltas.find(delta_name);
		if (delta_iter != std::end(type->deltas)) {
			throw ParserError{"Duplicated delta declaration '" +
					delta_name + "'", ast_delta.type};
		}

		// insert delta
		type->deltas.insert({delta_name, type_iter->second});
	}
}

void NyanSpecAnalyzer::analyze_attributes(const ASTNyanType &ast_type,
		std::shared_ptr<NyanType> type) {
	// for each attribute in the AST
	for (auto &ast_attr : ast_type.attributes) {
		// determine the attribute's name
		auto &attr_name = ast_attr.name.content;

		// check if an attribute with the same name already exists
		auto attr_iter = type->attributes.find(attr_name);
		if (attr_iter != std::end(type->attributes)) {
			throw ParserError{"Duplicate attribute declaration '" + attr_name +
					"'", ast_attr.name};
		}

		auto &attr_type = ast_attr.type.content;
		std::shared_ptr<NyanDatatype> datatype;
		if (ast_attr.is_set) {
			auto type_iter = spec->types.find(attr_type);
			if (type_iter == std::end(spec->types)) {
				throw ParserError{"Unknown type '" + attr_type + "'",
						ast_attr.type};
			}
			datatype = NyanSet::get(type_iter->second);
		} else {
			if (attr_type == "bool") {
				datatype = NyanBool::get();
			} else if (attr_type == "int") {
				datatype = NyanInt::get();
			} else if (attr_type == "float") {
				datatype = NyanFloat::get();
			} else if (attr_type == "string") {
				datatype = NyanString::get();
			} else {
				auto type_iter = spec->types.find(attr_type);
				if (type_iter == std::end(spec->types)) {
					throw ParserError{"Unknown type '" + attr_type + "'",
							ast_attr.type};
				}
				datatype = NyanCustomType::get(type_iter->second);
			}
		}
		auto attr = std::make_shared<NyanAttribute>(attr_name, datatype);
		type->attributes.insert({attr_name, attr});
	}
}

}
