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

class array_out_of_range final: public document_error {
public:
	array_out_of_range()
	    : document_error("�z��I�u�W�F�N�g�̗v�f�����z���菭�Ȃ��ł��B") {}
};
class onstream_array_out_of_range final
    : public position_indicatable_error<onstream_array_out_of_range> {
public:
	explicit onstream_array_out_of_range(std::streampos position)
	    : position_indicatable_error(
	          position, "�z��I�u�W�F�N�g�̗v�f�����z���菭�Ȃ��ł��B") {}
};

class dictionary_out_of_range final: public document_error {
public:
	dictionary_out_of_range()
	    : document_error("�z��I�u�W�F�N�g�̗v�f�����z���菭�Ȃ��ł��B") {}
};
class onstream_dictionary_out_of_range final
    : public position_indicatable_error<onstream_dictionary_out_of_range> {
public:
	explicit onstream_dictionary_out_of_range(std::streampos position)
	    : position_indicatable_error(
	          position, "�z��I�u�W�F�N�g�̗v�f�����z���菭�Ȃ��ł��B") {}
};
} // namespace pdfparser
