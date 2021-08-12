#include "literal_trim.hpp"

std::string operator""_trimmed(const char* str, std::size_t length) {
	std::string trimmed_str(str, length);
	if ('\n' == trimmed_str.front()) {
		trimmed_str.erase(trimmed_str.begin());
	}
	if ('\n' == trimmed_str.back()) {
		trimmed_str.pop_back();
	}
	return trimmed_str;
}
