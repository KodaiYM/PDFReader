#pragma once

#include "pdfparser.document_error.hpp"

namespace pdfparser {
class promise_token_failed final
    : public position_indicatable_error<promise_token_failed> {
public:
	template <class InputIterator>
	promise_token_failed(std::streampos position, InputIterator first,
	                     InputIterator last);

private:
	template <class InputIterator>
	static std::string generate_message(InputIterator first, InputIterator last);
};
} // namespace pdfparser

#include "pdfparser.tokenizer_errors.ipp"
