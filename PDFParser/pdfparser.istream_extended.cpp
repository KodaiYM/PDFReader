#include "pdfparser.istream_extended.hpp"

using namespace pdfparser;

pdfparser::istream_extended::istream_extended(std::streambuf* sb)
    : basic_istream(sb) {
	exceptions(failbit | badbit);
}

[[nodiscard]] std::optional<char> istream_extended::peek() noexcept {
	if (eof()) {
		return std::nullopt;
	}

	return traits_type::to_char_type(std::istream::peek());
}
[[nodiscard]] char istream_extended::get() {
	auto next_ch = peek();
	++*this;
	return next_ch.value();
}
[[nodiscard]] bool istream_extended::eof() noexcept {
	return std::istream::eof() ||
	       traits_type::eq_int_type(traits_type::eof(), std::istream::peek());
}

[[nodiscard]] std::streamoff istream_extended::tell() const noexcept {
	return rdbuf()->pubseekoff(0, std::ios_base::cur, std::ios_base::in);
}
void istream_extended::seek(std::streamoff byte_offset) {
	try {
		seekg(byte_offset, std::ios_base::beg);
	} catch (std::ios_base::failure&) {
		throw istream_extended_error(istream_extended_error::failed_to_seek);
	}
}
void istream_extended::seek_to_end() noexcept {
	// NOTE: undefined behaviour on ISO C but it will go well on Windows
	seekg(0, std::ios_base::end);
}
void istream_extended::seek_forward_head_of_line() {
	// check an immediately preceding newline character
	try {
		seekg(-1, std::ios_base::cur);
	} catch (std::ios_base::failure&) {
		throw istream_extended_error(
		    istream_extended_error::failed_to_seek_forward_head_of_line);
	}
	assert(good());
	switch (std::istream::peek()) {
	case '\r':
		// preceding newline character is CR
		break;
	case '\n':
		try {
			seekg(-1, std::ios_base::cur);
		} catch (std::ios_base::failure&) {
			// there is only LF in front of stream
			clear();
			return;
		}

		if ('\r' == std::istream::peek()) {
			// preceding newline character is CRLF
			// do nothing
		} else {
			// preceding newline character is LF
			// assert: noexcept
			seekg(1, std::ios_base::cur);
		}
		break;
	}

	// seek to the next character after the previous newline character or
	// beginning of the stream
	do {
		try {
			seekg(-1, std::ios_base::cur);
		} catch (std::ios_base::failure&) {
			// beginnig of the stream
			clear();
			return;
		}
		assert(good());
	} while (std::istream::peek() != '\r' && std::istream::peek() != '\n');

	// advance the stream by one character equivalent to '\r' or '\n'
	// assert: noexcept
	seekg(1, std::ios_base::cur);
}
istream_extended& istream_extended::operator++() {
	try {
		seekg(1, std::ios_base::cur);
	} catch (std::ios_base::failure&) {
		throw istream_extended_error(istream_extended_error::failed_to_seek);
	}
	return *this;
}

void istream_extended::ignore_if_present(whitespace_flags flags) noexcept {
	constexpr auto check_one_character = [](char ch) noexcept {
		return [ch](istream_extended& stream) noexcept {
			if (stream.attempt(std::string{ch})) {
				return true;
			} else {
				return false;
			}
		};
	};
	std::vector<std::function<bool(istream_extended&)>> ignore_functions;
	if ((whitespace_flags::null & flags) != whitespace_flags{}) {
		ignore_functions.push_back(check_one_character('\0'));
	}
	if ((whitespace_flags::horizontal_tab & flags) != whitespace_flags{}) {
		ignore_functions.push_back(check_one_character('\t'));
	}
	if ((whitespace_flags::line_feed & flags) != whitespace_flags{}) {
		ignore_functions.push_back(check_one_character('\n'));
	}
	if ((whitespace_flags::form_feed & flags) != whitespace_flags{}) {
		ignore_functions.push_back(check_one_character('\f'));
	}
	if ((whitespace_flags::carriage_return & flags) != whitespace_flags{}) {
		ignore_functions.push_back(check_one_character('\r'));
	}
	if ((whitespace_flags::space & flags) != whitespace_flags{}) {
		ignore_functions.push_back(check_one_character(' '));
	}
	if ((whitespace_flags::comment & flags) != whitespace_flags{}) {
		ignore_functions.push_back([](istream_extended& stream) noexcept {
			if (!stream.attempt("%")) {
				return false;
			}

			// skip comment contents
			std::optional<char> next_ch;
			while ((next_ch = stream.peek()).has_value() && next_ch != '\r' &&
			       next_ch != '\n') {
				++stream;
			}
			return true;
		});
	}

	// repeat while any of ignore_functions returns true
	while (std::any_of(
	    ignore_functions.cbegin(), ignore_functions.cend(),
	    [this](std::function<bool(istream_extended&)> ignore_fn) noexcept {
		    return ignore_fn(*this);
	    }))
		; // do nothing
}
bool istream_extended::attempt(std::string_view attempt_str) noexcept {
	const auto old_pos = tell();
	for (auto attempt_char : attempt_str) {
		if (attempt_char == peek()) {
			++*this;
		} else {
			// restore the position of the stream to the position
			// at which this function is called
			seek(old_pos);
			return false;
		}
	}

	return true;
}
void istream_extended::promise(
    std::initializer_list<std::string_view> promise_list) {
	if (std::any_of(
	        promise_list.begin(), promise_list.end(),
	        std::bind(&istream_extended::attempt, this, std::placeholders::_1))) {
		// found promise_str
		// do nothing
		return;
	} else {
		// not found any of them
		throw istream_extended_error(istream_extended_error::promise_failed);
	}
}
