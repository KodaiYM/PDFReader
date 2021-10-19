#pragma once

#include "pdfparser.document_error.hpp"

namespace pdfparser {
class onstream_type_mismatch final: public position_indicatable_error {
public:
	explicit onstream_type_mismatch(std::streampos position)
	    : position_indicatable_error(position, "�^�������܂���") {}
};
class type_mismatch final: public document_error {
public:
	type_mismatch() : document_error("�^�������܂���") {}
};
} // namespace pdfparser
