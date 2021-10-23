#pragma once

#include "pdfparser.istream_extended_errors.hpp"

#include <algorithm>
#include <sstream>

using namespace pdfparser;

template <class InputIterator>
promise_failed::promise_failed(std::streampos position, InputIterator first,
                               InputIterator last)
    : position_indicatable_error(position, generate_message(first, last)) {}
template <class InputIterator>
std::string promise_failed::generate_message(InputIterator first,
                                             InputIterator last) {
	std::stringstream message;
	message << "ˆÈ‰º‚Ì•¶Žš—ñ‚Ì‚¢‚¸‚ê‚©‚Ì•¶Žš—ñ‚ðŠú‘Ò‚µ‚Ü‚µ‚½‚ªA‚¢‚¸‚ê‚àŽæ“¾‚Å‚«"
	           "‚Ü‚¹‚ñ‚Å‚µ‚½B\n";

	std::for_each(first, last, [&message](std::string_view promise_str) {
		message << promise_str;

		message << '(';

		auto old_flags =
		    message.setf(std::ios_base::hex | std::ios_base::uppercase);
		for (const auto& character : promise_str) {
			message << static_cast<unsigned int>(character);
		}
		message.flags(old_flags);

		message << ')';

		message << '\n';
	});

	return message.str();
}
