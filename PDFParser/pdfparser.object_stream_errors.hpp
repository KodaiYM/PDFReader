#pragma once

#include "pdfparser.document_error.hpp"

#include <string>

namespace pdfparser {
class boolean_object_not_found final
    : public position_indicatable_error<boolean_object_not_found> {
public:
	explicit boolean_object_not_found(std::streampos position)
	    : position_indicatable_error(position,
	                                 "Boolean �I�u�W�F�N�g���K�v�ł��B") {}
};
class integer_object_not_found final
    : public position_indicatable_error<integer_object_not_found> {
public:
	explicit integer_object_not_found(std::streampos position)
	    : position_indicatable_error(position,
	                                 "Integer �I�u�W�F�N�g���K�v�ł��B") {}
};
class real_object_not_found final
    : public position_indicatable_error<real_object_not_found> {
public:
	explicit real_object_not_found(std::streampos position)
	    : position_indicatable_error(position, "Real �I�u�W�F�N�g���K�v�ł��B") {}
};
class string_object_not_found final
    : public position_indicatable_error<string_object_not_found> {
public:
	explicit string_object_not_found(std::streampos position)
	    : position_indicatable_error(position,
	                                 "String �I�u�W�F�N�g���K�v�ł��B") {}
};
class name_object_not_found final
    : public position_indicatable_error<name_object_not_found> {
public:
	explicit name_object_not_found(std::streampos position)
	    : position_indicatable_error(position, "Name �I�u�W�F�N�g���K�v�ł��B") {}
};
class array_object_not_found final
    : public position_indicatable_error<array_object_not_found> {
public:
	explicit array_object_not_found(std::streampos position)
	    : position_indicatable_error(position, "Array �I�u�W�F�N�g���K�v�ł��B") {
	}
};
class dictionary_object_not_found final
    : public position_indicatable_error<dictionary_object_not_found> {
public:
	explicit dictionary_object_not_found(std::streampos position)
	    : position_indicatable_error(position,
	                                 "Dictionary �I�u�W�F�N�g���K�v�ł��B") {}
};
class stream_object_not_found final
    : public position_indicatable_error<stream_object_not_found> {
public:
	explicit stream_object_not_found(std::streampos position)
	    : position_indicatable_error(position,
	                                 "Stream �I�u�W�F�N�g���K�v�ł��B") {}
};
class null_object_not_found final
    : public position_indicatable_error<null_object_not_found> {
public:
	explicit null_object_not_found(std::streampos position)
	    : position_indicatable_error(position, "Null �I�u�W�F�N�g���K�v�ł��B") {}
};
class indirect_reference_not_found final
    : public position_indicatable_error<indirect_reference_not_found> {
public:
	explicit indirect_reference_not_found(std::streampos position)
	    : position_indicatable_error(position, "�ԐڎQ�Ƃ��K�v�ł��B") {}
};
// TODO: �����ۗ���
class specified_object_not_found final
    : public position_indicatable_error<specified_object_not_found> {
public:
	explicit specified_object_not_found(std::streampos position)
	    : position_indicatable_error(position, "message") {}
};

class real_object_overflows final
    : public position_indicatable_error<real_object_overflows> {
public:
	explicit real_object_overflows(std::streampos position)
	    : position_indicatable_error(
	          position, "�����I�u�W�F�N�g�̓����\�����I�[�o�[�t���[���܂����B") {}
};

class literal_string_lack_of_right_parenthesis final
    : public position_indicatable_error<
          literal_string_lack_of_right_parenthesis> {
public:
	explicit literal_string_lack_of_right_parenthesis(std::streampos position)
	    : position_indicatable_error(position,
	                                 "�����񃊃e�����̏I�[�� ) ������܂���B") {}
};
class array_lack_of_right_square_bracket final
    : public position_indicatable_error<array_lack_of_right_square_bracket> {
public:
	explicit array_lack_of_right_square_bracket(std::streampos position)
	    : position_indicatable_error(position, "�z��̏I�[�� ] ������܂���B") {}
};
class hexadecimal_string_non_hexadecimal_digit_found final
    : public position_indicatable_error<
          hexadecimal_string_non_hexadecimal_digit_found> {
public:
	explicit hexadecimal_string_non_hexadecimal_digit_found(
	    std::streampos position)
	    : position_indicatable_error(
	          position, "16�i�����񃊃e�����ɔ�16�i����������܂��B") {}
};
class hexadecimal_string_lack_of_greater_than_sign final
    : public position_indicatable_error<
          hexadecimal_string_lack_of_greater_than_sign> {
public:
	explicit hexadecimal_string_lack_of_greater_than_sign(std::streampos position)
	    : position_indicatable_error(
	          position, "16�i�����񃊃e�����̏I�[�� > ������܂���B") {}
};
class dictionary_lack_of_double_greater_than_sign final
    : public position_indicatable_error<
          dictionary_lack_of_double_greater_than_sign> {
public:
	explicit dictionary_lack_of_double_greater_than_sign(std::streampos position)
	    : position_indicatable_error(position, "�����̏I�[�� >> ������܂���B") {
	}
};
class stream_dictionary_absence_of_Length_entry final
    : public position_indicatable_error<
          stream_dictionary_absence_of_Length_entry> {
public:
	explicit stream_dictionary_absence_of_Length_entry(std::streampos position)
	    : position_indicatable_error(
	          position,
	          "�X�g���[���I�u�W�F�N�g�̎����� Length �G���g��������܂���B") {}
};
class stream_data_is_shorter_than_Length final
    : public position_indicatable_error<stream_data_is_shorter_than_Length> {
public:
	stream_data_is_shorter_than_Length(std::streampos  position,
	                                   std::streamsize length)
	    : position_indicatable_error(
	          position, "�X�g���[���I�u�W�F�N�g�̃f�[�^�� Length �G���g���̒l(" +
	                        std::to_string(length) + ")�ɔ�ׂĒZ�����܂��B") {}
};
class indirect_object_is_inconsistent_with_xref_table final
    : public position_indicatable_error<
          indirect_object_is_inconsistent_with_xref_table> {
public:
	explicit indirect_object_is_inconsistent_with_xref_table(
	    std::streampos position, xref_types::object_t object_number,
	    xref_types::generation_t            generation_number,
	    const xref_types::xref_inuse_entry& object_info)
	    : position_indicatable_error(
	          position, "���̊ԐڃI�u�W�F�N�g�̃I�u�W�F�N�g�ԍ�(" +
	                        std::to_string(object_number) + ")�Ɛ���ԍ�(" +
	                        std::to_string(generation_number) +
	                        ")�́A"
	                        "�N���X���t�@�����X���(" +
	                        std::to_string(object_info.object_number) + ", " +
	                        std::to_string(object_info.generation_number) +
	                        +") �Ɩ������Ă��܂��B") {}
};
} // namespace pdfparser
