#pragma once

#include "PDFParser.error_types.h"
#include "PDFParser.object_types.h"
#include "PDFParser.xref_types.h"

#include <algorithm>
#include <array>
#include <cassert>
#include <charconv>
#include <fstream>

namespace pdfparser {
template <class InputStreamT>
class object_pool;

enum class require_type {
	keyword_EOF,
	EOL,
	keyword_startxref,
	keyword_xref,
	space
};

enum class ignore_flag : uint8_t {
	null                      = 1 << 0,
	horizontal_tab            = 1 << 1,
	line_feed                 = 1 << 2,
	form_feed                 = 1 << 3,
	carriage_return           = 1 << 4,
	space                     = 1 << 5,
	comment                   = 1 << 6,
	EOL                       = line_feed | carriage_return,
	any_whitespace_characters = null | horizontal_tab | line_feed | form_feed |
	                            carriage_return | space | EOL,
	any_whitespace_characters_except_EOL = any_whitespace_characters & ~EOL
};

template <class InputStreamT>
class stream_parser {
	static_assert(std::is_base_of_v<std::istream, InputStreamT>,
	              "template parameter InputStreamT must be a derived class of "
	              "std::istream");

public:
	constexpr void seek_to_end();

	constexpr std::streamoff tell();

	constexpr void seek(std::streamoff byte_offset_from_beginning_of_file);

	/// <exception cref="std::ios_base::failure">
	/// thrown when there is no beginning of line frontward
	/// </exception>
	constexpr void seek_forward_head_of_line();

	/// <summary>
	/// take byte offset of cross reference table from footer
	/// </summary>
	/// <exceptions cref="std::overflow_error">
	/// </exceptions>
	/// <exceptions cref="pdfparser::error_types::parse_error"></exceptions>
	/// <returns>byte offset of cross reference table</returns>
	constexpr std::streamoff take_xref_byte_offset();

	/// <summary>
	/// take cross reference table
	/// </summary>
	/// <exceptions cref="std::overflow_error">
	/// </exceptions>
	/// <exceptions cref="pdfparser::error_types::parse_error"></exceptions>
	/// <returns>byte offset of cross reference table</returns>
	constexpr xref_types::xref_table take_xref_table();

	/// <exceptions cref="pdfparser::error_types::parse_error">
	/// thrown when invalid format detected
	/// </exceptions>
	constexpr xref_types::xref_entry
	    take_xref_entry(xref_types::object_t object_number);

	constexpr object_types::dictionary_object
	    take_trailer(object_pool<InputStreamT>& object_accessor);

	constexpr object_types::any_direct_object_or_ref
	    take_any_object(object_pool<InputStreamT>& object_accessor);

	constexpr object_types::any_direct_object
	    take_indirect_object(object_pool<InputStreamT>& object_accessor,
	                         xref_types::object_t       object_number,
	                         xref_types::generation_t   generation_number);

	/// <summary>
	/// consume token specified by req_type
	/// if it's not present, throw parse_error
	/// </summary>
	/// <exceptions cref="pdfparser::error_types::parse_error">
	/// thrown when specified token is not present.
	/// </exceptions>
	constexpr void require(require_type req_type);

	/// <summary>
	/// ignore whitespaces specified by flags if they are present on m_stream.
	/// </summary>
	/// <param name="flags">whitespace bit flags to be ignored</param>
	constexpr void ignore_if_present(ignore_flag flags);

	/// <exception cref="pdfparser::error_types::parse_error">
	/// thrown when m_stream cannot be interpreted as a signed integer
	/// </exception>
	/// <exception cref="std::overflow_error">
	/// thrown when the integer is overflow
	/// </exception>
	template <typename IntType>
	constexpr IntType take_signed_integer();

	/// <exception cref="pdfparser::error_types::parse_error">
	/// thrown when m_stream cannot be interpreted as an unsigned integer
	/// </exception>
	/// <exception cref="std::overflow_error">
	/// thrown when the integer is overflow
	/// </exception>
	template <typename IntType>
	constexpr IntType take_unsigned_integer();

public:
	constexpr explicit stream_parser(InputStreamT&& stream);
	stream_parser(InputStreamT&)        = delete;
	stream_parser(const InputStreamT&&) = delete;

private:
	InputStreamT m_stream;
};
} // namespace pdfparser

/* definition of inline functions */
namespace pdfparser {
#pragma region ignore_flag_operators
constexpr ignore_flag operator&(ignore_flag lhs, ignore_flag rhs) noexcept {
	using int_type = std::underlying_type_t<ignore_flag>;
	return static_cast<ignore_flag>(static_cast<int_type>(lhs) &
	                                static_cast<int_type>(rhs));
}
constexpr ignore_flag operator|(ignore_flag lhs, ignore_flag rhs) noexcept {
	using int_type = std::underlying_type_t<ignore_flag>;
	return static_cast<ignore_flag>(static_cast<int_type>(lhs) |
	                                static_cast<int_type>(rhs));
}
constexpr ignore_flag operator^(ignore_flag lhs, ignore_flag rhs) noexcept {
	using int_type = std::underlying_type_t<ignore_flag>;
	return static_cast<ignore_flag>(static_cast<int_type>(lhs) ^
	                                static_cast<int_type>(rhs));
}
constexpr ignore_flag& operator&=(ignore_flag& lhs, ignore_flag rhs) noexcept {
	return lhs = lhs & rhs;
}
constexpr ignore_flag& operator|=(ignore_flag& lhs, ignore_flag rhs) noexcept {
	return lhs = lhs | rhs;
}
constexpr ignore_flag& operator^=(ignore_flag& lhs, ignore_flag rhs) noexcept {
	return lhs = lhs ^ rhs;
}
constexpr ignore_flag operator~(ignore_flag operand) noexcept {
	using int_type = std::underlying_type_t<ignore_flag>;
	return static_cast<ignore_flag>(~static_cast<int_type>(operand));
}
#pragma endregion // region ignore_flag_operators
} // namespace pdfparser

/* definition of template functions */
namespace pdfparser {
#pragma region stream_parser_definitions
template <class InputStreamT>
constexpr stream_parser<InputStreamT>::stream_parser(InputStreamT&& stream)
    : m_stream(std::move(stream)) {
	m_stream.clear();
	m_stream.exceptions(std::ios_base::failbit | std::ios_base::badbit);
}

// TODO: Implement! seek_to_end
// TODO: Implement! tell
// TODO: Implement! seek

template <class InputStreamT>
constexpr void stream_parser<InputStreamT>::seek_forward_head_of_line() {
	// immediately preceding newline character
	try {
		m_stream.seekg(-1, std::ios_base::cur);
	} catch (const std::ios_base::failure&) {
		throw parse_error(parse_error::failed_to_seek_forward_head_of_line);
	}
	assert(m_stream.good());
	switch (m_stream.peek()) {
		case '\r':
			break;
		case '\n':
			try {
				m_stream.seekg(-1, std::ios_base::cur);
			} catch (std::ios_base::failure&) {
				m_stream.clear();
				return;
			}

			if (m_stream.peek() != '\r') {
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
			m_stream.clear();
			return;
		}
		assert(m_stream.good());
	} while (m_stream.peek() != '\r' && m_stream.peek() != '\n');

	// assert: noexcept
	m_stream.seekg(1, std::ios_base::cur);
}

template <class InputStreamT>
constexpr std::streamoff stream_parser<InputStreamT>::take_xref_byte_offset() {
	ignore_if_present(ignore_flag::any_whitespace_characters_except_EOL);

	std::streamoff xref_byte_offset;
	try {
		xref_byte_offset = take_unsigned_integer<std::streamoff>();
	} catch (parse_error& e) {
		if (e.code() == parse_error::unsigned_integer_not_found) {
			throw parse_error(parse_error::xref_byte_offset_not_found);
		} else {
			throw;
		}
	}

	ignore_if_present(ignore_flag::any_whitespace_characters_except_EOL |
	                  ignore_flag::comment);
	require(require_type::EOL);
	return xref_byte_offset;
}

template <class InputStreamT>
constexpr xref_types::xref_table
    stream_parser<InputStreamT>::take_xref_table() {
	using namespace xref_types;

	xref_table this_xref_table;
	require(require_type::keyword_xref);
	const object_t first_object_number = take_unsigned_integer<object_t>();
	require(require_type::space);
	const object_t number_of_entries = take_unsigned_integer<object_t>();

	assert(number_of_entries > 0); // HACK: error check? throw?
	// this "if statement" means...
	// first_object_number + number_of_entries - 1 >
	// std::numeric_limits<object_t>::max()
	if (number_of_entries - 1 >
	    std::numeric_limits<object_t>::max() - first_object_number) {
		throw std::overflow_error("overflow");
	}
	ignore_if_present(ignore_flag::any_whitespace_characters_except_EOL |
	                  ignore_flag::comment);
	require(require_type::EOL);
	for (object_t entry_offset = 0; entry_offset < number_of_entries;
	     ++entry_offset) {
		const auto object_number = first_object_number + entry_offset;
		this_xref_table.insert(take_xref_entry(object_number));
	}
	return this_xref_table;
}

template <class InputStreamT>
constexpr xref_types::xref_entry stream_parser<InputStreamT>::take_xref_entry(
    xref_types::object_t object_number) {
	char first_10_digits[10];
	try {
		m_stream.read(first_10_digits, 10);
	} catch (std::ios_base::failure&) {
		throw parse_error(parse_error::xref_entry_first_10_digits_invalid);
	}
	if (!std::all_of(std::cbegin(first_10_digits), std::cend(first_10_digits),
	                 [](char digit) { return std::isdigit(digit); })) {
		throw parse_error(parse_error::xref_entry_first_10_digits_invalid);
	}

	require(require_type::space);

	char second_5_digits[5];
	try {
		m_stream.read(second_5_digits, 5);
	} catch (std::ios_base::failure&) {
		throw parse_error(parse_error::xref_entry_second_5_digits_invalid);
	}
	if (!std::all_of(std::cbegin(second_5_digits), std::cend(second_5_digits),
	                 [](char digit) { return std::isdigit(digit); })) {
		throw parse_error(parse_error::xref_entry_second_5_digits_invalid);
	}

	require(require_type::space);

	char keyword;
	try {
		m_stream.read(&keyword, 1);
	} catch (std::ios_base::failure&) {
		throw parse_error(parse_error::xref_entry_keyword_invalid);
	}
	if (keyword != 'n' && keyword != 'f') {
		throw parse_error(parse_error::xref_entry_keyword_invalid);
	}

	char last_2_bytes[2];
	try {
		m_stream.read(last_2_bytes, 2);
	} catch (std::ios_base::failure&) {
		throw parse_error(parse_error::xref_entry_last_2_bytes_invalid);
	}
	if (std::string_view last_2_bytes_sv{last_2_bytes, 2};
	    last_2_bytes_sv != " \r" && last_2_bytes_sv != " \n" &&
	    last_2_bytes_sv != "\r\n") {
		throw parse_error(parse_error::xref_entry_last_2_bytes_invalid);
	}

	xref_types::generation_t generation_number;
	{
		auto generation_conv_result = std::from_chars(
		    second_5_digits, second_5_digits + 5, generation_number, 10);
		assert(generation_conv_result.ec == std::errc{} ||
		       generation_conv_result.ec == std::errc::result_out_of_range);
		if (std::errc::result_out_of_range == generation_conv_result.ec) {
			throw std::overflow_error("overflow");
		}
	}

	if ('n' == keyword) {
		decltype(xref_types::xref_inuse_entry::byte_offset) byte_offset;
		auto byte_offset_conv_result =
		    std::from_chars(first_10_digits, first_10_digits + 10, byte_offset, 10);
		assert(byte_offset_conv_result.ec == std::errc{} ||
		       byte_offset_conv_result.ec == std::errc::result_out_of_range);
		if (std::errc::result_out_of_range == byte_offset_conv_result.ec) {
			throw std::overflow_error("overflow");
		}
		return xref_types::xref_inuse_entry{object_number, generation_number,
		                                    byte_offset};
	} else {
		assert('f' == keyword);

		decltype(xref_types::xref_free_entry::next_free_object_number)
		     next_free_object_number;
		auto next_free_object_number_conv_result = std::from_chars(
		    first_10_digits, first_10_digits + 10, next_free_object_number, 10);
		assert(next_free_object_number_conv_result.ec == std::errc{} ||
		       next_free_object_number_conv_result.ec ==
		           std::errc::result_out_of_range);
		if (std::errc::result_out_of_range ==
		    next_free_object_number_conv_result.ec) {
			throw std::overflow_error("overflow");
		}
		return xref_types::xref_free_entry{object_number, generation_number,
		                                   next_free_object_number};
	}
}

// TODO: Implement! take_trailer
// TODO: Implement! take_any_object
// TODO: Implement! take_indirect_object

template <class InputStreamT>
constexpr void stream_parser<InputStreamT>::require(require_type req_type) {
	const auto attempt = [](std::istream&    m_stream,
	                        std::string_view attempt_str) noexcept -> bool {
		if (m_stream.eof()) {
			return false;
		}

		const auto old_pos = m_stream.tellg();
		for (auto attempt_char : attempt_str) {
			if (attempt_char == m_stream.peek()) {
				m_stream.seekg(1, std::ios_base::cur);
			} else {
				m_stream.seekg(old_pos);
				return false;
			}
		}

		return true;
	};

	switch (req_type) {
		case require_type::keyword_EOF:
			if (m_stream.eof()) {
				throw parse_error(parse_error::keyword_EOF_not_found);
			}

			if (!attempt(m_stream, "%%EOF")) {
				throw parse_error(parse_error::keyword_EOF_not_found);
			}

			if (std::decay_t<decltype(m_stream)>::traits_type::eof() ==
			    m_stream.peek()) {
				return;
			}

			require(require_type::EOL);
			break;
		case require_type::EOL:
			if (m_stream.eof()) {
				throw parse_error(parse_error::EOL_not_found);
			}

			if (attempt(m_stream, "\n")) {
				// do nothing
			} else if (attempt(m_stream, "\r")) {
				attempt(m_stream, "\n");
			} else {
				throw parse_error(parse_error::EOL_not_found);
			}
			break;
		case require_type::keyword_startxref:
			if (m_stream.eof()) {
				throw parse_error(parse_error::keyword_startxref_not_found);
			}

			ignore_if_present(ignore_flag::any_whitespace_characters_except_EOL);
			if (!attempt(m_stream, "startxref")) {
				throw parse_error(parse_error::keyword_startxref_not_found);
			}
			ignore_if_present(ignore_flag::any_whitespace_characters_except_EOL |
			                  ignore_flag::comment);
			require(require_type::EOL);
			break;
		case require_type::keyword_xref:
			if (m_stream.eof()) {
				throw parse_error(parse_error::keyword_xref_not_found);
			}

			ignore_if_present(ignore_flag::any_whitespace_characters_except_EOL);
			if (!attempt(m_stream, "xref")) {
				throw parse_error(parse_error::keyword_xref_not_found);
			}
			ignore_if_present(ignore_flag::any_whitespace_characters_except_EOL |
			                  ignore_flag::comment);
			require(require_type::EOL);
			break;
		case require_type::space:
			if (m_stream.eof()) {
				throw parse_error(parse_error::space_not_found);
			}

			if (!attempt(m_stream, " ")) {
				throw parse_error(parse_error::space_not_found);
			}
			break;
	}
}

template <class InputStreamT>
constexpr void
    stream_parser<InputStreamT>::ignore_if_present(ignore_flag flags) {
	if (m_stream.eof()) {
		return;
	}

	using istream_t = std::istream;

	std::vector<std::function<bool(istream_t&)>> ignore_functions;
	{
		// constant (will be moved)
		std::array<std::pair<ignore_flag, std::function<bool(istream_t&)>>, 7>
		    ignore_functions_map{
		        {{ignore_flag::null,
		          [](istream_t& m_stream) {
			          if ('\0' == m_stream.peek()) {
				          m_stream.seekg(1, std::ios_base::cur);
				          return true;
			          } else {
				          return false;
			          }
		          }},
		         {ignore_flag::horizontal_tab,
		          [](istream_t& m_stream) {
			          if ('\t' == m_stream.peek()) {
				          m_stream.seekg(1, std::ios_base::cur);
				          return true;
			          } else {
				          return false;
			          }
		          }},
		         {ignore_flag::line_feed,
		          [](istream_t& m_stream) {
			          if ('\n' == m_stream.peek()) {
				          m_stream.seekg(1, std::ios_base::cur);
				          return true;
			          } else {
				          return false;
			          }
		          }},
		         {ignore_flag::form_feed,
		          [](istream_t& m_stream) {
			          if ('\f' == m_stream.peek()) {
				          m_stream.seekg(1, std::ios_base::cur);
				          return true;
			          } else {
				          return false;
			          }
		          }},
		         {ignore_flag::carriage_return,
		          [](istream_t& m_stream) {
			          if ('\r' == m_stream.peek()) {
				          m_stream.seekg(1, std::ios_base::cur);
				          return true;
			          } else {
				          return false;
			          }
		          }},
		         {ignore_flag::space,
		          [](istream_t& m_stream) {
			          if (' ' == m_stream.peek()) {
				          m_stream.seekg(1, std::ios_base::cur);
				          return true;
			          } else {
				          return false;
			          }
		          }},
		         {ignore_flag::comment, [](istream_t& m_stream) {
			          if (m_stream.peek() != '%') {
				          return false;
			          }
			          m_stream.seekg(1, std::ios_base::cur);

			          // skip comment contents
			          while (m_stream.peek() != istream_t::traits_type::eof() &&
			                 m_stream.peek() != '\r' && m_stream.peek() != '\n') {
				          m_stream.seekg(1, std::ios_base::cur);
			          }
			          return true;
		          }}}};

		// generate ignore_functions
		for (auto&& [flag, function] : ignore_functions_map) {
			if ((flag & flags) != ignore_flag{}) {
				ignore_functions.push_back(std::move(function));
			}
		}
	}

	bool ignored = true;
	while (ignored && m_stream.peek() != istream_t::traits_type::eof()) {
		ignored = false;
		for (const auto& ignore_a_whitespace : ignore_functions) {
			if (ignore_a_whitespace(m_stream)) {
				ignored = true;
				break;
			}
		}
	}
}

template <class InputStreamT>
template <typename IntType>
constexpr IntType stream_parser<InputStreamT>::take_signed_integer() {
	if (m_stream.eof()) {
		throw parse_error(parse_error::signed_integer_not_found);
	}

	bool has_sign = false;
	switch (m_stream.peek()) {
		case std::decay_t<decltype(m_stream)>::traits_type::eof():
			throw parse_error(parse_error::signed_integer_not_found);
		case '+':
		case '-':
			has_sign = true;
			// assert: noexcept
			m_stream.seekg(1, std::ios_base::cur);
			break;
	}

	// if m_stream does not begin with any of "0123456789"
	if (m_stream.peek() == std::decay_t<decltype(m_stream)>::traits_type::eof() ||
	    !std::isdigit(static_cast<unsigned char>(m_stream.peek()))) {
		throw parse_error(parse_error::signed_integer_not_found);
	}

	if (has_sign) {
		// assert: noexcept
		m_stream.seekg(-1, std::ios_base::cur);
	}

	IntType integer;
	try {
		m_stream >> integer;
	} catch (std::ios_base::failure&) { throw std::overflow_error("overflow"); }

	return integer;
}

template <class InputStreamT>
template <typename IntType>
constexpr IntType stream_parser<InputStreamT>::take_unsigned_integer() {
	if (m_stream.eof()) {
		throw parse_error(parse_error::unsigned_integer_not_found);
	}

	// if m_stream does not begin with any of "0123456789"
	if (m_stream.peek() == std::decay_t<decltype(m_stream)>::traits_type::eof() ||
	    !std::isdigit(static_cast<unsigned char>(m_stream.peek()))) {
		throw parse_error(parse_error::unsigned_integer_not_found);
	}

	IntType integer;
	try {
		m_stream >> integer;
	} catch (std::ios_base::failure&) { throw std::overflow_error("overflow"); }

	return integer;
}
#pragma endregion // region stream_parser_definitions
} // namespace pdfparser
