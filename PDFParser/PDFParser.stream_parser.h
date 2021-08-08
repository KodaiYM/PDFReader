#pragma once

#include "PDFParser.error_types.h"
#include "PDFParser.object_types.h"
#include "PDFParser.xref_types.h"

#include <cassert>
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
public:
	void           seek_to_end();
	std::streamoff tell();
	void           seek(std::streamoff byte_offset_from_beginning_of_file);
	void           seek_forward_head_of_line();
	std::streamoff take_xref_byte_offset();
	xref_types::xref_table take_xref_table();
	xref_types::xref_entry take_xref_entry(xref_types::object_t object_number);
	object_types::dictionary_object
	    take_trailer(object_pool<InputStreamT>& object_accessor);
	object_types::any_direct_object_or_ref
	    take_any_object(object_pool<InputStreamT>& object_accessor);
	object_types::any_direct_object
	     take_indirect_object(object_pool<InputStreamT>& object_accessor,
	                          xref_types::object_t       object_number,
	                          xref_types::generation_t   generation_number);
	void require(require_type req_type);
	void ignore_if_present(ignore_flag flags);
	template <typename IntType>
	IntType take_signed_integer();
	template <typename IntType>
	IntType take_unsigned_integer();

public:
	explicit stream_parser(InputStreamT&& stream);
	stream_parser(InputStreamT&)        = delete;
	stream_parser(const InputStreamT&&) = delete;

private:
	InputStreamT m_stream;
};
constexpr ignore_flag  operator&(ignore_flag lhs, ignore_flag rhs) noexcept;
constexpr ignore_flag  operator|(ignore_flag lhs, ignore_flag rhs) noexcept;
constexpr ignore_flag  operator^(ignore_flag lhs, ignore_flag rhs) noexcept;
constexpr ignore_flag& operator&=(ignore_flag& lhs, ignore_flag rhs) noexcept;
constexpr ignore_flag& operator|=(ignore_flag& lhs, ignore_flag rhs) noexcept;
constexpr ignore_flag& operator^=(ignore_flag& lhs, ignore_flag rhs) noexcept;
constexpr ignore_flag  operator~(ignore_flag operand) noexcept;
} // namespace pdfparser

/* definitions of template functions */
namespace pdfparser {
/// <exception cref="pdfparser::error_types::parse_error">
/// thrown when m_stream cannot be interpreted as a signed integer
/// </exception>
/// <exception cref="std::overflow_error">
/// thrown when the integer is overflow or underflow
/// </exception>
template <class InputStreamT>
template <typename IntType>
IntType stream_parser<InputStreamT>::take_signed_integer() {
	assert(m_stream.exceptions() ==
	       (std::ios_base::badbit | std::ios_base::failbit));
	assert(!m_stream.fail());

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

/// <exception cref="pdfparser::error_types::parse_error">
/// thrown when m_stream cannot be interpreted as an unsigned integer
/// </exception>
/// <exception cref="std::overflow_error">
/// thrown when the integer is overflow
/// </exception>
template <class InputStreamT>
template <typename IntType>
IntType stream_parser<InputStreamT>::take_unsigned_integer() {
	assert(m_stream.exceptions() ==
	       (std::ios_base::badbit | std::ios_base::failbit));
	assert(!m_stream.fail());

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
} // namespace pdfparser
