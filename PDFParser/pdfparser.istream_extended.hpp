#pragma once

#include "pdfparser.istream_extended_error.hpp"
#include "pdfparser.whitespace_flags.hpp"

#include <algorithm>
#include <cassert>
#include <fstream> // for IntelliSense
#include <functional>
#include <ios>
#include <optional>
#include <string_view>
#include <vector>

namespace pdfparser {
template <class InputStreamT>
class istream_extended {
public:
	/// <summary>peek next character</summary>
	/// <returns>
	/// std::nullopt for the end of file, otherwise the next character
	/// </returns>
	[[nodiscard]] std::optional<typename InputStreamT::char_type> peek() noexcept;

	/// <summary>get next character with peek() and do ++*this</summary>
	/// <returns>
	/// next character
	/// </returns>
	/// <exception cref="pdfparser::istream_extended_error(failed_to_seek)">
	/// thrown when failed to ++*this
	/// </exception>
	[[nodiscard]] typename InputStreamT::char_type get();

	/// <summary>check whether end of file or not</summary>
	/// <returns>true if and only if stream is on EOF</returns>
	[[nodiscard]] bool eof() noexcept;

	/// <summary>
	/// get current stream position (byte offset from beginning of file)
	/// </summary>
	/// <returns>current byte offset from beginning of file</returns>
	[[nodiscard]] typename InputStreamT::off_type tell() const noexcept;

	/// <summary>
	/// move stream position to byte_offset_from_beginning_of_file
	/// </summary>
	/// <param name="byte_offset">byte offset from beginning of file</param>
	/// <exception cref="pdfparser::istream_extended_error(failed_to_seek)">
	/// thrown with error code failed_to_seek when failed to seek
	/// </exception>
	void seek(typename InputStreamT::off_type byte_offset);

	/// <summary>seek to end of stream</summary>
	void seek_to_end() noexcept;

	/// <summary>
	/// If the current position is the beginning of a line,
	///  seek to the beginning of the previous line;
	/// otherwise, seek to the beginning of the current line.
	/// </summary>
	/// <exception
	/// cref="pdfparser::istream_extended_error(failed_to_seek_forward_head_of_line)">
	/// thrown when the current position is the beginning of the stream
	/// </exception>
	void seek_forward_head_of_line();

	/// <summary>advance the stream by one character</summary>
	/// <returns>*this</returns>
	/// <exception cref="pdfparser::istream_extended_error(failed_to_seek)">
	/// thrown when failed to seek
	/// </exception>
	istream_extended& operator++();

	/// <summary>
	/// ignore whitespaces specified by flags if they are present on m_stream.
	/// </summary>
	/// <param name="flags">
	/// bit flags to specify which whitespace characters to ignore
	/// </param>
	void ignore_if_present(whitespace_flags flags) noexcept;

	/// <summary>
	/// consume attempt_str at the current position of the stream if it's present;
	/// otherwise, do nothing.
	/// </summary>
	/// <param name="attempt_str">string trying to consume</param>
	/// <returns>true if and only if consumed</returns>
	bool attempt(std::string_view attempt_str) noexcept;

	/// <summary>
	/// Promise any string in the promise_list to be able to attempt.
	/// If none of the strings can be attempted, it throws
	/// istreram_extended_error(promised_failed).
	/// </summary>
	/// <param name="promise_list">
	/// list of strings which one of them is promised to be able to attempt
	/// </param>
	/// <exception cref="pdfparser::istream_extended_error(promise_failed)">
	/// thrown when none of the strings can be attempted
	/// </exception>
	void promise(std::initializer_list<std::string_view> promise_list);

public:
	static_assert(std::is_base_of_v<std::istream, InputStreamT>,
	              "template parameter InputStreamT must be a derived class of "
	              "std::istream");

	/// <summary>move the argument stream and construct</summary>
	/// <param name="stream">input stream inheriting from std::istream</param>
	/// <exception cref="std::ios_base::failure">
	/// thrown when stream.rdbuf() == nullptr
	/// </exception>
	/// <exception>
	/// move constructor of InputStreamT may throw an exception.
	/// </exception>
	istream_extended(InputStreamT&& stream);

	/// <summary>prohibit to move from const rvalue/summary>
	istream_extended(const InputStreamT&&) = delete;

	/// <summary>prohibit to copy from lvalue</summary>
	istream_extended(InputStreamT&) = delete;

	virtual ~istream_extended() = default;

private:
	InputStreamT m_stream;
};
} // namespace pdfparser

/// <summary>
/// Definition of member functions of class istream_extended
/// </summary>
namespace pdfparser {
#pragma region special_functions_of_istream_extended
template <class InputStreamT>
istream_extended<InputStreamT>::istream_extended(InputStreamT&& stream)
    : m_stream(std::move(stream)) {
	m_stream.clear();
	m_stream.exceptions(std::ios_base::failbit | std::ios_base::badbit);
}
#pragma endregion // region special_functions_of_istream_extended

#pragma region member_functions_of_istream_extended
template <class InputStreamT>
[[nodiscard]] typename InputStreamT::char_type
    istream_extended<InputStreamT>::get() {
	auto next_ch = peek();
	++*this;
	return next_ch.value();
}
template <class InputStreamT>
[[nodiscard]] std::optional<typename InputStreamT::char_type>
    istream_extended<InputStreamT>::peek() noexcept {
	if (eof()) {
		return std::nullopt;
	}

	return typename InputStreamT::traits_type::to_char_type(m_stream.peek());
}
template <class InputStreamT>
[[nodiscard]] bool istream_extended<InputStreamT>::eof() noexcept {
	return m_stream.eof() ||
	       InputStreamT::traits_type::eq_int_type(
	           InputStreamT::traits_type::eof(), m_stream.peek());
}

template <class InputStreamT>
[[nodiscard]] typename InputStreamT::off_type
    istream_extended<InputStreamT>::tell() const noexcept {
	return m_stream.rdbuf()->pubseekoff(0, std::ios_base::cur, std::ios_base::in);
}
template <class InputStreamT>
void istream_extended<InputStreamT>::seek(
    typename InputStreamT::off_type byte_offset) {
	try {
		m_stream.seekg(byte_offset, std::ios_base::beg);
	} catch (std::ios_base::failure&) {
		throw istream_extended_error(istream_extended_error::failed_to_seek);
	}
}
template <class InputStreamT>
void istream_extended<InputStreamT>::seek_to_end() noexcept {
	// NOTE: undefined behaviour on ISO C but it will go well on Windows
	m_stream.seekg(0, std::ios_base::end);
}
template <class InputStreamT>
void istream_extended<InputStreamT>::seek_forward_head_of_line() {
	// check an immediately preceding newline character
	try {
		m_stream.seekg(-1, std::ios_base::cur);
	} catch (std::ios_base::failure&) {
		throw istream_extended_error(
		    istream_extended_error::failed_to_seek_forward_head_of_line);
	}
	assert(m_stream.good());
	switch (m_stream.peek()) {
	case '\r':
		// preceding newline character is CR
		break;
	case '\n':
		try {
			m_stream.seekg(-1, std::ios_base::cur);
		} catch (std::ios_base::failure&) {
			// there is only LF in front of stream
			m_stream.clear();
			return;
		}

		if ('\r' == m_stream.peek()) {
			// preceding newline character is CRLF
			// do nothing
		} else {
			// preceding newline character is LF
			// assert: noexcept
			m_stream.seekg(1, std::ios_base::cur);
		}
		break;
	}

	// seek to the next character after the previous newline character or
	// beginning of the stream
	do {
		try {
			m_stream.seekg(-1, std::ios_base::cur);
		} catch (std::ios_base::failure&) {
			// beginnig of the stream
			m_stream.clear();
			return;
		}
		assert(m_stream.good());
	} while (m_stream.peek() != '\r' && m_stream.peek() != '\n');

	// advance the stream by one character equivalent to '\r' or '\n'
	// assert: noexcept
	m_stream.seekg(1, std::ios_base::cur);
}
template <class InputStreamT>
auto istream_extended<InputStreamT>::operator++() -> istream_extended& {
	try {
		m_stream.seekg(1, std::ios_base::cur);
	} catch (std::ios_base::failure&) {
		throw istream_extended_error(istream_extended_error::failed_to_seek);
	}
	return *this;
}

template <class InputStreamT>
void istream_extended<InputStreamT>::ignore_if_present(
    whitespace_flags flags) noexcept {
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
template <class InputStreamT>
bool istream_extended<InputStreamT>::attempt(
    std::string_view attempt_str) noexcept {
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
template <class InputStreamT>
void istream_extended<InputStreamT>::promise(
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
#pragma endregion // region member_functions_of_istream_extended
} // namespace pdfparser
