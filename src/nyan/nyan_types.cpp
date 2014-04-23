#include "nyan_types.h"

#include "nyan_spec.h"
#include "util.h"

namespace nyan {

std::string NyanBool::to_string() const {
	return "bool";
}

std::shared_ptr<NyanDatatype> NyanBool::get() {
	static auto instance = std::make_shared<NyanBool>();
	return instance;
}

std::string NyanInt::to_string() const {
	return "int";
}

std::shared_ptr<NyanDatatype> NyanInt::get() {
	static auto instance = std::make_shared<NyanInt>();
	return instance;
}

std::string NyanFloat::to_string() const {
	return "float";
}

std::shared_ptr<NyanDatatype> NyanFloat::get() {
	static auto instance = std::make_shared<NyanFloat>();
	return instance;
}

std::string NyanString::to_string() const {
	return "string";
}

std::shared_ptr<NyanDatatype> NyanString::get() {
	static auto instance = std::make_shared<NyanString>();
	return instance;
}

std::string NyanCustomType::to_string() const {
	std::shared_ptr<NyanType> s_type{type.lock()};
	return s_type->name;
}

std::shared_ptr<NyanDatatype> NyanCustomType::get(
		std::shared_ptr<NyanType> type) {
	static std::unordered_map<NyanType*,std::shared_ptr<NyanDatatype>>
			instances;
	NyanType *type_ptr = type.get();
	auto inst_iter = instances.find(type_ptr);
	if (inst_iter == std::end(instances)) {
		auto instance = std::make_shared<NyanCustomType>(type);
		instances.insert({type_ptr, instance});
		return instance;
	}
	return inst_iter->second;
}

std::string NyanSet::to_string() const {
	std::shared_ptr<NyanType> s_type{type.lock()};
	return "set(" + s_type->name + ")";
}

std::shared_ptr<NyanDatatype> NyanSet::get(std::shared_ptr<NyanType> type) {
	static std::unordered_map<NyanType*,std::shared_ptr<NyanDatatype>>
			instances;
	NyanType *type_ptr = type.get();
	auto inst_iter = instances.find(type_ptr);
	if (inst_iter == std::end(instances)) {
		auto instance = std::make_shared<NyanSet>(type);
		instances.insert({type_ptr, instance});
		return instance;
	}
	return inst_iter->second;
}

NyanCustomType::NyanCustomType(std::shared_ptr<NyanType> type)
		:
		type{type} {
}

NyanSet::NyanSet(std::shared_ptr<NyanType> type)
		:
		type{type} {
}

}
