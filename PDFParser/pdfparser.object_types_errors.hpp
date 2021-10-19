#pragma once

#include "pdfparser.document_error.hpp"

namespace pdfparser {
class onstream_type_mismatch final: public position_indicatable_error {
public:
	explicit onstream_type_mismatch(std::streampos position)
	    : position_indicatable_error(position, "Œ^‚ª‡‚¢‚Ü‚¹‚ñ") {}
};
class type_mismatch final: public document_error {
public:
	type_mismatch() : document_error("Œ^‚ª‡‚¢‚Ü‚¹‚ñ") {}
};
} // namespace pdfparser
