#pragma once

#include "pdfparser.tokenizer_errors.hpp"

#include <algorithm>
#include <sstream>

namespace pdfparser {

template <class InputIterator>
promise_token_failed::promise_token_failed(std::streampos position,
                                           InputIterator  first,
                                           InputIterator  last)
    : position_indicatable_error(position, generate_message(first, last)) {}

template <class InputIterator>
std::string promise_token_failed::generate_message(InputIterator first,
                                                   InputIterator last) {
	std::stringstream message;
	message << "以下のいずれかのトークンを期待しましたが、いずれも取得でき"
	           "ませんでした。\n";

	std::for_each(first, last, [&message](std::string_view promise_token_str) {
		message << promise_token_str;

		message << '(';

		auto old_flags =
		    message.setf(std::ios_base::hex | std::ios_base::uppercase);
		for (const auto& character : promise_token_str) {
			message << static_cast<unsigned int>(character);
		}
		message.flags(old_flags);

		message << ')';

		message << '\n';
	});

	return message.str();
}

} // namespace pdfparser
