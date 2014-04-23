#ifndef _NYAN_UTIL_H_
#define _NYAN_UTIL_H_

#include <string>
#include <vector>

namespace nyan {

const std::vector<std::string> BASIC_TYPES = {
		"bool", "int", "float", "string"
	};

std::string replace_controls(const std::string &s);

bool is_basic_type_name(const std::string &name);

}

#endif
