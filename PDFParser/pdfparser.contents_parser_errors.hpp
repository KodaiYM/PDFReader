#pragma once

#include "pdfparser.document_error.hpp"

#include <string>
#include <string_view>

namespace pdfparser {
class unknown_operator: public position_indicatable_error<unknown_operator> {
public:
	unknown_operator(std::string_view operator_str, std::streampos position)
	    : position_indicatable_error(position,
	                                 std::string(operator_str) +
	                                     std::string(": ïsñæÇ»ââéZéqÇ≈Ç∑ÅB")) {}
};
} // namespace pdfparser
