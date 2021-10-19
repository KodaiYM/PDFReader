#pragma once

#include "pdfparser.document_error.hpp"

namespace pdfparser {
class xref_entry_keyword_invalid final: public position_indicatable_error {
public:
	explicit xref_entry_keyword_invalid(std::streampos position)
	    : position_indicatable_error(
	          position,
	          "�N���X���t�@�����X�G���g���ɖ����ȃL�[���[�h������܂��B") {}
};
class object_number_overflow_in_xref_table final
    : public position_indicatable_error {
public:
	explicit object_number_overflow_in_xref_table(std::streampos position)
	    : position_indicatable_error(position,
	                                 "���̃N���X���t�@�����X�Z�N�V������"
	                                 "�I�u�W�F�N�g�ԍ����I�[�o�[�t���[���܂��B") {
	}
};
} // namespace pdfparser
