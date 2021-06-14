#include "PDFParser.error_types.h"
#include "PDFParser.parser.h"

#include <bitset>
#include <climits>

static_assert(CHAR_BIT == 8, "CHAR_BIT != 8");

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
	m_stream.exceptions(std::ios_base::failbit | std::ios_base::badbit);
}

/****************
  Parser Utility
 ****************/
#define EOF_ EOF
#undef EOF
enum class require_type { EOF, EOL, startxref, xref, space };
#define EOF EOF_

enum class ignore_flag {
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
using ignore_flag_bitset_t = std::bitset<7>;

// TODO: implement following functions

static void seek_to_frontward_beginning_of_line(std::istream& istr) {
	// immediately preceding newline character
	istr.seekg(-1, std::ios_base::cur);
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
	} while (istr.peek() != '\r' && istr.peek() != '\n');
	istr.seekg(1, std::ios_base::cur);
}

static std::streamoff take_xref_byte_offset(std::istream& istr);

static xref_types::xref_table take_xref_table(std::istream& istr);

static xref_types::xref_entry take_xref_entry(std::istream& istr);

static void require(std::istream& istr, require_type req_type);

static void ignore_if_present(std::istream&               istr,
                              const ignore_flag_bitset_t& flags);

template <typename SignedIntType>
static SignedIntType take_signed_integer(std::istream& istr);

template <typename UnsignedIntType>
static UnsignedIntType take_unsigned_integer(std::istream& istr);

/**********************
  parser::footer Class
 **********************/
parser::footer::footer(std::istream& istr) {
	// check %%EOF
	try {
		istr.seekg(0, std::ios_base::end);
		seek_to_frontward_beginning_of_line(istr);
	} catch (std::ios_base::failure&) {
		throw error_types::syntax_error(error_types::syntax_error::EOF_not_found);
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
	  throw error_types::syntax_error(
	      error_types::syntax_error::xref_byte_offset_not_found);
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
	  throw error_types::syntax_error(
	      error_types::syntax_error::keyword_startxref_not_found);
	}
	require(istr, require_type::startxref);

	// get cross-reference table
	istr.seekg(xref_byte_offset, std::ios_base::beg);
	xref_table = take_xref_table(istr);*/
}
} // namespace pdfparser
