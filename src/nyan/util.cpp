#include "util.h"

#include <sstream>

namespace nyan {

std::string replace_controls(const std::string &s) {
	std::string result;
	for (auto c : s) {
		switch (c) {
		case 0x0:
			result += "\\0";
			break;
		case 0x7:
			result += "\\a";
			break;
		case 0x8:
			result += "\\b";
			break;
		case 0x9:
			result += "\\t";
			break;
		case 0xA:
			result += "\\n";
			break;
		case 0xB:
			result += "\\v";
			break;
		case 0xC:
			result += "\\f";
			break;
		case 0xD:
			result += "\\r";
			break;
		default:
			if (c <= 0x1F) {
				std::ostringstream n;
				n << "\\x" << std::hex << static_cast<int>(c);
				result += n.str();
			} else {
				result += c;
			}
			break;
		}
	}
	return result;
}


}
