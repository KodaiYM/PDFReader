#pragma once

#include "pdfparser.document_error.hpp"

namespace pdfparser {
class type_mismatch final: public document_error {
public:
	type_mismatch() : document_error("�^�������܂���") {}
};
class onstream_type_mismatch final
    : public position_indicatable_error<onstream_type_mismatch> {
public:
	explicit onstream_type_mismatch(std::streampos position)
	    : position_indicatable_error(position, "�^�������܂���") {}
};

class integer_object_overflows final: public document_error {
public:
	integer_object_overflows()
	    : document_error("�����I�u�W�F�N�g���I�[�o�[�t���[���܂����B") {}
};
class onstream_integer_object_overflows final
    : public position_indicatable_error<onstream_integer_object_overflows> {
public:
	explicit onstream_integer_object_overflows(std::streampos position)
	    : position_indicatable_error(
	          position, "�����I�u�W�F�N�g���I�[�o�[�t���[���܂����B") {}
};
} // namespace pdfparser
