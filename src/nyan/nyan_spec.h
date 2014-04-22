#ifndef _NYAN_NYAN_SPEC_H_
#define _NYAN_NYAN_SPEC_H_

#include <memory>
#include <string>
#include <unordered_map>

#include "nyan_types.h"

namespace nyan {

struct NyanAttribute {
	std::string name;
	std::shared_ptr<NyanDatatype> type;

	NyanAttribute(const std::string &name, std::shared_ptr<NyanDatatype> type);
	~NyanAttribute() = default;

	std::string to_string() const;
};

struct NyanType {
	std::string name;

	std::unordered_map<std::string,std::shared_ptr<NyanAttribute>> attributes;
	bool allow_dynamic_attributes;
	std::unordered_map<std::string,std::weak_ptr<NyanType>> deltas;

	NyanType(const std::string &name, bool allow_dynamic_attributes);
	~NyanType() = default;

	std::string to_string() const;
};

struct NyanSpec {
	std::unordered_map<std::string,std::shared_ptr<NyanType>> types;
	
	NyanSpec() = default;
	~NyanSpec() = default;

	std::string to_string() const;
};

}

#endif
