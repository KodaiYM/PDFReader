#pragma once

#include "pdfparser.document_error.hpp"

#include <string>

namespace pdfparser {
class failed_to_get final: public position_indicatable_error {
public:
	explicit failed_to_get(std::streampos position)
	    : position_indicatable_error(position, "���̕����̎擾�Ɏ��s���܂����B") {
	}
};
class failed_to_seek final: public document_error {
public:
	explicit failed_to_seek(std::streampos seekpos)
	    : document_error(std::to_string(seekpos) +
	                     "�o�C�g�ڂɃV�[�N�o���܂���ł����B") {}
};
class failed_to_seek_forward_head_of_line final
    : public position_indicatable_error {
public:
	explicit failed_to_seek_forward_head_of_line(std::streampos position)
	    : position_indicatable_error(position,
	                                 "�O�̍s�ɃV�[�N�o���܂���ł����B") {}
};
class promise_failed final: public position_indicatable_error {
public:
	template <class InputIterator>
	promise_failed(std::streampos position, InputIterator first,
	               InputIterator last);

private:
	template <class InputIterator>
	static std::string generate_message(InputIterator first, InputIterator last);
};
} // namespace pdfparser

#include "pdfparser.istream_extended_errors.ipp"
