#include "nyan_spec.h"

namespace nyan {

NyanAttribute::NyanAttribute(const std::string &name,
		std::shared_ptr<NyanDatatype> type)
		:
		name{name},
		type{type} {
}

std::string NyanAttribute::to_string() const {
	std::string result;
	result += name + " : " + type->to_string();
	return result;
}

NyanType::NyanType(const std::string &name, bool allow_dynamic_attributes)
		:
		name{name},
		allow_dynamic_attributes{allow_dynamic_attributes} {
}

std::string NyanType::to_string() const {
	std::string result;
	result += name + " {\n";
	for (auto &attr : attributes) {
		result += "\t" + attr.second->to_string() + "\n";
	}
	if (allow_dynamic_attributes) {
		result += "\t...\n";
	}
	for (auto &delta : deltas) {
		result += "\t^" + delta.first + "\n";
	}
	result += "}";
	return result;
}

std::string NyanSpec::to_string() const {
	std::string result;
	for (auto &type : types) {
		result += type.second->to_string() + "\n";
	}
	return result;
}

}
