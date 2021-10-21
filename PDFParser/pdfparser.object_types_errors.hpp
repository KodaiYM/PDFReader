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

class array_out_of_range final: public document_error {
public:
	array_out_of_range()
	    : document_error("配列オブジェクトの要素数が想定より少ないです。") {}
};
class array_onstream_out_of_range final
    : public position_indicatable_error<array_onstream_out_of_range> {
public:
	explicit array_onstream_out_of_range(std::streampos position)
	    : position_indicatable_error(
	          position, "配列オブジェクトの要素数が想定より少ないです。") {}
};
} // namespace pdfparser
