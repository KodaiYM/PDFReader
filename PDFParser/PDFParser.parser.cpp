#include "PDFParser.error_types.h"
#include "PDFParser.parser.h"

#include <array>
#include <bitset>
#include <cassert>
#include <cctype>
#include <climits>
#include <functional>
#include <set>
#include <sstream>
#include <string>
#include <type_traits>
#include <vector>

static_assert(CHAR_BIT == 8, "CHAR_BIT != 8");
using namespace std::string_literals;

/**************
  Parser Class
 **************/
namespace pdfparser {
const xref_types::xref_table& parser::get_xref_table() & {
	if (!m_footer) {
		m_footer = std::make_unique<footer>(m_stream);
	}
	return m_footer->xref_table;
}
xref_types::xref_table parser::get_xref_table() && {
	this->get_xref_table();
	return std::move(m_footer->xref_table);
}
template <class FilenameT>
parser::parser(const FilenameT& filename)
    : m_stream(filename, std::ios_base::binary) {
	m_stream.exceptions(
	    std::ios_base::failbit |
	    std::ios_base::badbit); // HACK: handle file open exception.
}

/****************
  Parser Utility
 ****************/
#undef EOF
enum class require_type { EOF, EOL, startxref, xref, space };

enum class ignore_flag : uint8_t {
	null                      = 1,
	horizontal_tab            = 2,
	line_feed                 = 4,
	form_feed                 = 8,
	carriage_return           = 16,
	space                     = 32,
	comment                   = 64,
	EOL                       = line_feed | carriage_return,
	any_whitespace_characters = null | horizontal_tab | line_feed | form_feed |
	                            carriage_return | space | EOL,
	any_whitespace_characters_except_EOL = any_whitespace_characters & ~EOL
};

/// <exception cref="std::ios_base::failure">
/// thrown when there is no beginning of line frontward
/// </exception>
static void seek_to_frontward_beginning_of_line(std::istream& istr) {
	assert(istr.exceptions() == (std::ios_base::badbit | std::ios_base::failbit));
	assert(istr.rdstate() == std::ios_base::goodbit);

	// immediately preceding newline character
	istr.seekg(-1, std::ios_base::cur); // throws std::ios_base::failure
	assert(istr.good());
	switch (istr.peek()) {
		case '\r':
			break;
		case '\n':
			try {
				istr.seekg(-1, std::ios_base::cur);
			} catch (std::ios_base::failure&) { istr.clear(); }
			break;
	}

	// seek to the next character after the previous newline character or
	// beginning of the stream
	do {
		try {
			istr.seekg(-1, std::ios_base::cur);
		} catch (std::ios_base::failure&) {
			istr.clear();
			return;
		}
		assert(istr.good());
	} while (istr.peek() != '\r' && istr.peek() != '\n');

	// assert: noexcept
	istr.seekg(1, std::ios_base::cur);
}

// TODO: implement following functions

static std::streamoff take_xref_byte_offset(std::istream& istr);

static xref_types::xref_table take_xref_table(std::istream& istr);

static xref_types::xref_entry take_xref_entry(std::istream& istr);

static void require(std::istream& istr, require_type req_type);

/// <summary>
/// ignore whitespaces specified on flags if present on istr.
/// </summary>
/// <param name="flags">whitespace bit flags to be ignored</param>
static void ignore_if_present(std::istream&                       istr,
                              std::underlying_type_t<ignore_flag> flags) {
	assert(istr.exceptions() == (std::ios_base::badbit | std::ios_base::failbit));
	assert(istr.rdstate() == std::ios_base::goodbit);

	using istream_t = std::istream;

	std::vector<std::function<bool(istream_t&)>> ignore_functions;
	{
		// constant (moved)
		std::array<std::pair<ignore_flag, std::function<bool(istream_t&)>>, 7>
		    ignore_functions_map{
		        {{ignore_flag::null,
		          [](istream_t& istr) {
			          if ('\0' == istr.peek()) {
				          istr.seekg(1, std::ios_base::cur);
				          return true;
			          } else {
				          return false;
			          }
		          }},
		         {ignore_flag::horizontal_tab,
		          [](istream_t& istr) {
			          if ('\t' == istr.peek()) {
				          istr.seekg(1, std::ios_base::cur);
				          return true;
			          } else {
				          return false;
			          }
		          }},
		         {ignore_flag::line_feed,
		          [](istream_t& istr) {
			          if ('\n' == istr.peek()) {
				          istr.seekg(1, std::ios_base::cur);
				          return true;
			          } else {
				          return false;
			          }
		          }},
		         {ignore_flag::form_feed,
		          [](istream_t& istr) {
			          if ('\f' == istr.peek()) {
				          istr.seekg(1, std::ios_base::cur);
				          return true;
			          } else {
				          return false;
			          }
		          }},
		         {ignore_flag::carriage_return,
		          [](istream_t& istr) {
			          if ('\r' == istr.peek()) {
				          istr.seekg(1, std::ios_base::cur);
				          return true;
			          } else {
				          return false;
			          }
		          }},
		         {ignore_flag::space,
		          [](istream_t& istr) {
			          if (' ' == istr.peek()) {
				          istr.seekg(1, std::ios_base::cur);
				          return true;
			          } else {
				          return false;
			          }
		          }},
		         {ignore_flag::comment, [](istream_t& istr) {
			          if (istr.peek() != '%') {
				          return false;
			          }
			          istr.seekg(1, std::ios_base::cur);

			          // skip comment contents
			          while (istr.peek() != istream_t::traits_type::eof() &&
			                 istr.peek() != '\r' && istr.peek() != '\n') {
				          istr.seekg(1, std::ios_base::cur);
			          }
			          return true;
		          }}}};

		// generate ignore_functions
		for (auto&& [flag, function] : ignore_functions_map) {
			if (static_cast<std::underlying_type_t<ignore_flag>>(flag) & flags) {
				ignore_functions.push_back(std::move(function));
			}
		}
	}

	bool ignored = true;
	while (ignored && istr.peek() != istream_t::traits_type::eof()) {
		ignored = false;
		for (const auto& ignore_a_whitespace : ignore_functions) {
			if (ignore_a_whitespace(istr)) {
				ignored = true;
				break;
			}
		}
	}
}

/// <exception cref="pdfparser::error_types::syntax_error">
/// thrown when istr cannot be interpreted as a signed integer
/// </exception>
/// <exception cref="pdfparser::error_types::overflow_or_underflow_error">
/// thrown when the integer is overflow or underflow
/// </exception>
template <typename SignedIntType,
          typename std::enable_if_t<std::is_signed_v<SignedIntType>,
                                    std::nullptr_t> = nullptr>
static SignedIntType take_signed_integer(std::istream& istr) {
	assert(istr.exceptions() == (std::ios_base::badbit | std::ios_base::failbit));
	assert(istr.rdstate() == std::ios_base::goodbit);

	bool has_sign = false;
	switch (istr.peek()) {
		case std::remove_reference_t<decltype(istr)>::traits_type::eof():
			throw syntax_error(syntax_error::signed_integer_not_found);
		case '+':
		case '-':
			has_sign = true;
			// assert: noexcept
			istr.seekg(1, std::ios_base::cur);
			break;
	}

	// if istr does not begin with any of "0123456789"
	if (istr.peek() ==
	        std::remove_reference_t<decltype(istr)>::traits_type::eof() ||
	    !std::isdigit(static_cast<unsigned char>(istr.peek()))) {
		throw syntax_error(syntax_error::signed_integer_not_found);
	}

	if (has_sign) {
		// assert: noexcept
		istr.seekg(-1, std::ios_base::cur);
	}

	SignedIntType signed_integer;
	try {
		istr >> signed_integer;
	} catch (std::ios_base::failure&) { throw overflow_or_underflow_error(); }

	return signed_integer;
}

/// <exception cref="pdfparser::error_types::syntax_error">
/// thrown when istr cannot be interpreted as an unsigned integer
/// </exception>
/// <exception cref="pdfparser::error_types::overflow_or_underflow_error">
/// thrown when the integer is overflow
/// </exception>
template <typename UnsignedIntType,
          typename std::enable_if_t<std::is_unsigned_v<UnsignedIntType>,
                                    std::nullptr_t> = nullptr>
static UnsignedIntType take_unsigned_integer(std::istream& istr) {
	assert(istr.exceptions() == (std::ios_base::badbit | std::ios_base::failbit));
	assert(istr.rdstate() == std::ios_base::goodbit);

	// if istr does not begin with any of "0123456789"
	if (istr.peek() ==
	        std::remove_reference_t<decltype(istr)>::traits_type::eof() ||
	    !std::isdigit(static_cast<unsigned char>(istr.peek()))) {
		throw syntax_error(syntax_error::unsigned_integer_not_found);
	}

	UnsignedIntType unsigned_integer;
	try {
		istr >> unsigned_integer;
	} catch (std::ios_base::failure&) { throw overflow_or_underflow_error(); }

	return unsigned_integer;
}

/**********************
  parser::footer Class
 **********************/
parser::footer::footer(std::istream& istr) {
	assert(istr.exceptions() == (std::ios_base::badbit | std::ios_base::failbit));
	assert(istr.rdstate() == std::ios_base::goodbit);

	// check %%EOF
	try {
		istr.seekg(0, std::ios_base::end);
		seek_to_frontward_beginning_of_line(istr);
	} catch (std::ios_base::failure&) {
		throw syntax_error(syntax_error::EOF_not_found);
	} /*
	{
	  auto eof_pos = istr.tellg();
	  require(istr, require_type::EOF);
	  istr.seekg(eof_pos);
	}

	// get cross-reference table byte offset
	try {
	  seek_to_frontward_beginning_of_line(istr);
	} catch (std::ios_base::failure&) {
	  throw syntax_error(
	      syntax_error::xref_byte_offset_not_found);
	}
	{
	  auto xref_byte_offset_pos = istr.tellg();
	  xref_byte_offset          = take_xref_byte_offset(istr);
	  istr.seekg(xref_byte_offset_pos);
	}

	// check keyword "startxref"
	try {
	  seek_to_frontward_beginning_of_line(istr);
	} catch (std::ios_base::failure&) {
	  throw syntax_error(
	      syntax_error::keyword_startxref_not_found);
	}
	require(istr, require_type::startxref);

	// get cross-reference table
	istr.seekg(xref_byte_offset, std::ios_base::beg);
	xref_table = take_xref_table(istr);*/
}
} // namespace pdfparser
