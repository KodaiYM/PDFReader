#pragma once

#include "pdfparser.document_error.hpp"

namespace pdfparser {
class type_mismatch final: public document_error {
public:
	type_mismatch() : document_error("型が合いません") {}
};
class onstream_type_mismatch final
    : public position_indicatable_error<onstream_type_mismatch> {
public:
	explicit onstream_type_mismatch(std::streampos position)
	    : position_indicatable_error(position, "型が合いません") {}
};

class integer_object_overflows final: public document_error {
public:
	integer_object_overflows()
	    : document_error("整数オブジェクトがオーバーフローしました。") {}
};
class onstream_integer_object_overflows final
    : public position_indicatable_error<onstream_integer_object_overflows> {
public:
	explicit onstream_integer_object_overflows(std::streampos position)
	    : position_indicatable_error(
	          position, "整数オブジェクトがオーバーフローしました。") {}
};
} // namespace pdfparser
