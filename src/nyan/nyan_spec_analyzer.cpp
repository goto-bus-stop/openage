#include "nyan_spec_analyzer.h"

#include "parser_error.h"
#include "util.h"

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
	// analyze the types
	analyze_types();
	// then analyze attributes and deltas
	analyze_type_bodies();
}

void NyanSpecAnalyzer::analyze_types() {
	for (auto &ast_type : ast->types) {
		auto &type_name = ast_type.name.content;

		// check whether a type with the same name already exists
		auto type_iter = spec->types.find(type_name);
		if (type_iter != std::end(spec->types)) {
			throw ParserError{"Redefinition of type '" + type_name + "'",
					ast_type.name};
		}

		// create the nyan type and add it to the spec
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
			throw ParserError{"Undefined type '" + delta_name + "'",
					ast_delta.type};
		}

		// check if a delta with the same name already exists
		auto delta_iter = type->deltas.find(delta_name);
		if (delta_iter != std::end(type->deltas)) {
			throw ParserError{"Redefinition of delta '" + delta_name + "'",
					ast_delta.type};
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
			throw ParserError{"Redefinition of attribute '" + attr_name +
					"'", ast_attr.name};
		}

		// determine the attribute's type
		auto &attr_type = ast_attr.type.content;
		std::shared_ptr<NyanDatatype> datatype;
		// if the attribute's type is a set
		if (ast_attr.is_set) {
			// check if the set type is defined
			auto type_iter = spec->types.find(attr_type);
			if (type_iter == std::end(spec->types)) {
				// the set type is a basic type, basic types are not allowed
				if (is_basic_type_name(attr_type)) {
					throw ParserError{"Basic types must not be used as "
							"set type: '" + attr_type + "'", ast_attr.type};
				} else  {
					throw ParserError{"Undefined type '" + attr_type + "'",
							ast_attr.type};
				}
			}
			datatype = NyanSet::get(type_iter->second);
		// the attribute's type is no set
		} else {
			// it is a bool
			if (attr_type == "bool") {
				datatype = NyanBool::get();
			// it is an int
			} else if (attr_type == "int") {
				datatype = NyanInt::get();
			// it is a float
			} else if (attr_type == "float") {
				datatype = NyanFloat::get();
			// it is a string
			} else if (attr_type == "string") {
				datatype = NyanString::get();
			// it is a nyan type
			} else {
				// check if the type is defined
				auto type_iter = spec->types.find(attr_type);
				if (type_iter == std::end(spec->types)) {
					throw ParserError{"Undefined type '" + attr_type + "'",
							ast_attr.type};
				}
				datatype = NyanCustomType::get(type_iter->second);
			}
		}
		// create and add the attribute to the nyan type
		auto attr = std::make_shared<NyanAttribute>(attr_name, datatype);
		type->attributes.insert({attr_name, attr});
	}
}

}
