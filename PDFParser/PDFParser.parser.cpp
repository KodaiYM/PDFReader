#include "PDFParser.parser.h"

#include <bitset>

/**************
  Parser Class
 **************/
namespace pdfparser {
template <class InputStreamT>
const xref_types::xref_table&
    parser<InputStreamT>::get_xref_table() const& noexcept {
	return m_footer.xref_table;
}
template <class InputStreamT>
xref_types::xref_table parser<InputStreamT>::get_xref_table() && {
	return std::move(m_footer.xref_table);
}
template <class InputStreamT>
parser<InputStreamT>::parser(InputStreamT&& istr)
    : m_stream(std::move(istr)), m_footer(m_stream) {}

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
template <class InputStreamT>
static std::streamoff take_xref_byte_offset(InputStreamT& istr);

template <class InputStreamT>
static xref_types::xref_table take_xref_table(InputStreamT& istr);

template <class InputStreamT>
static xref_types::xref_entry take_xref_entry(InputStreamT& istr);

template <class InputStreamT>
static void require(InputStreamT& istr, require_type req_type);

template <class InputStreamT>
static void ignore_if_present(InputStreamT&               istr,
                              const ignore_flag_bitset_t& flags);

template <typename SignedIntType, class InputStreamT>
static SignedIntType take_signed_integer(InputStreamT& istr);

template <typename UnsignedIntType, class InputStreamT>
static UnsignedIntType take_unsigned_integer(InputStreamT& istr);

/**********************
  parser::footer Class
 **********************/
template <class InputStreamT>
parser<InputStreamT>::footer::footer(InputStreamT& istr) {
	// TODO: take_footer
}
} // namespace pdfparser
