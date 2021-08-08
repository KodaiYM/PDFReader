#include "PDFParser.error_types.h"
#include "PDFParser.parser.h"

// For Windows min max macro
#define NOMINMAX

#include <algorithm>
#include <array>
#include <cassert>
#include <cctype>
#include <charconv>
#include <climits>
#include <functional>
#include <msclr/marshal_cppstd.h>
#include <set>
#include <sstream>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>

static_assert(CHAR_BIT == 8, "CHAR_BIT != 8");
using namespace std::string_literals;

namespace pdfparser {
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
/**************
  Parser Class
 **************/
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

/*******************************
  Parser C++/CLI toString Class
 *******************************/
System::String ^ parser_tostring::get(System::String ^ filename) {
	parser PDFParser{msclr::interop::marshal_as<std::string>(filename)};

	auto xref_table_str = System::String::Empty;
	auto xref_table     = PDFParser.get_xref_table();

	for (const auto& entry : xref_table) {
		using namespace xref_types;
		if (auto inuse_entry = std::get_if<xref_inuse_entry>(&entry)) {
			xref_table_str += System::String::Format(
			    "using ({0}, {1}) -> offset: {2}", inuse_entry->object_number,
			    inuse_entry->generation_number, inuse_entry->byte_offset);
		} else if (auto free_entry = std::get_if<xref_free_entry>(&entry)) {
			xref_table_str += System::String::Format(
			    "free {0} (next gen num to be used: {1}) -> next obj num: {2}",
			    free_entry->object_number, free_entry->next_used_generation_number,
			    free_entry->next_free_object_number);
		} else {
			assert(false);
		}
		xref_table_str += "\n";
	}
	return xref_table_str;
}

/* definitions of internal functions */
/// <exception cref="std::ios_base::failure">
/// thrown when there is no beginning of line frontward
/// </exception>
static void seek_forward_head_of_line(std::istream& istr) {
	assert(istr.exceptions() == (std::ios_base::badbit | std::ios_base::failbit));
	assert(!istr.fail());

	// immediately preceding newline character
	istr.seekg(-1, std::ios_base::cur); // throws std::ios_base::failure
	assert(istr.good());
	switch (istr.peek()) {
		case '\r':
			break;
		case '\n':
			try {
				istr.seekg(-1, std::ios_base::cur);
			} catch (std::ios_base::failure&) {
				istr.clear();
				return;
			}

			if (istr.peek() != '\r') {
				// assert: noexcept
				istr.seekg(1, std::ios_base::cur);
			}
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

/// <summary>
/// take byte offset of cross reference table from footer
/// </summary>
/// <exceptions cref="std::overflow_error">
/// </exceptions>
/// <exceptions cref="pdfparser::error_types::syntax_error"></exceptions>
/// <returns>byte offset of cross reference table</returns>
static std::streamoff take_xref_byte_offset(std::istream& istr) {
	ignore_if_present(istr, ignore_flag::any_whitespace_characters_except_EOL);

	std::streamoff xref_byte_offset;
	try {
		xref_byte_offset = take_unsigned_integer<std::streamoff>(istr);
	} catch (syntax_error& e) {
		if (e.code() == syntax_error::unsigned_integer_not_found) {
			throw syntax_error(syntax_error::xref_byte_offset_not_found);
		} else {
			throw;
		}
	}

	ignore_if_present(istr, ignore_flag::any_whitespace_characters_except_EOL |
	                            ignore_flag::comment);
	require(istr, require_type::EOL);
	return xref_byte_offset;
}

/// <summary>
/// take cross reference table
/// </summary>
/// <exceptions cref="std::overflow_error">
/// </exceptions>
/// <exceptions cref="pdfparser::error_types::syntax_error"></exceptions>
/// <returns>byte offset of cross reference table</returns>
static xref_types::xref_table take_xref_table(std::istream& istr) {
	using namespace xref_types;

	xref_table this_xref_table;
	require(istr, require_type::keyword_xref);
	const object_t first_object_number = take_unsigned_integer<object_t>(istr);
	require(istr, require_type::space);
	const object_t number_of_entries = take_unsigned_integer<object_t>(istr);

	assert(number_of_entries > 0); // HACK: error check? throw?
	// this if expression means...
	// first_object_number + number_of_entries - 1 >
	// std::numeric_limits<object_t>::max()
	if (number_of_entries - 1 >
	    std::numeric_limits<object_t>::max() - first_object_number) {
		throw std::overflow_error("overflow");
	}
	ignore_if_present(istr, ignore_flag::any_whitespace_characters_except_EOL |
	                            ignore_flag::comment);
	require(istr, require_type::EOL);
	for (object_t entry_offset = 0; entry_offset < number_of_entries;
	     ++entry_offset) {
		const auto object_number = first_object_number + entry_offset;
		this_xref_table.insert(take_xref_entry(istr, object_number));
	}
	return this_xref_table;
}

/// <exceptions cref="pdfparser::error_types::syntax_error">
/// thrown when invalid format detected
/// </exceptions>
static xref_types::xref_entry
    take_xref_entry(std::istream& istr, xref_types::object_t object_number) {
	assert(istr.exceptions() == (std::ios_base::badbit | std::ios_base::failbit));
	assert(!istr.fail());

	char first_10_digits[10];
	try {
		istr.read(first_10_digits, 10);
	} catch (std::ios_base::failure&) {
		throw syntax_error(syntax_error::xref_entry_first_10_digits_invalid);
	}
	if (!std::all_of(std::cbegin(first_10_digits), std::cend(first_10_digits),
	                 [](char digit) { return std::isdigit(digit); })) {
		throw syntax_error(syntax_error::xref_entry_first_10_digits_invalid);
	}

	require(istr, require_type::space);

	char second_5_digits[5];
	try {
		istr.read(second_5_digits, 5);
	} catch (std::ios_base::failure&) {
		throw syntax_error(syntax_error::xref_entry_second_5_digits_invalid);
	}
	if (!std::all_of(std::cbegin(second_5_digits), std::cend(second_5_digits),
	                 [](char digit) { return std::isdigit(digit); })) {
		throw syntax_error(syntax_error::xref_entry_second_5_digits_invalid);
	}

	require(istr, require_type::space);

	char keyword;
	try {
		istr.read(&keyword, 1);
	} catch (std::ios_base::failure&) {
		throw syntax_error(syntax_error::xref_entry_keyword_invalid);
	}
	if (keyword != 'n' && keyword != 'f') {
		throw syntax_error(syntax_error::xref_entry_keyword_invalid);
	}

	char last_2_bytes[2];
	try {
		istr.read(last_2_bytes, 2);
	} catch (std::ios_base::failure&) {
		throw syntax_error(syntax_error::xref_entry_last_2_bytes_invalid);
	}
	if (std::string_view last_2_bytes_sv{last_2_bytes, 2};
	    last_2_bytes_sv != " \r" && last_2_bytes_sv != " \n" &&
	    last_2_bytes_sv != "\r\n") {
		throw syntax_error(syntax_error::xref_entry_last_2_bytes_invalid);
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

/// <summary>
/// consume token specified by req_type
/// if it's not present, throw syntax_error
/// </summary>
/// <exceptions cref="pdfparser::error_types::syntax_error">
/// thrown when specified token is not present.
/// </exceptions>
static void require(std::istream& istr, require_type req_type) {
	assert(istr.exceptions() == (std::ios_base::badbit | std::ios_base::failbit));
	assert(!istr.fail());

	const auto attempt = [](std::istream&    istr,
	                        std::string_view attempt_str) noexcept -> bool {
		if (istr.eof()) {
			return false;
		}

		const auto old_pos = istr.tellg();
		for (auto attempt_char : attempt_str) {
			if (attempt_char == istr.peek()) {
				istr.seekg(1, std::ios_base::cur);
			} else {
				istr.seekg(old_pos);
				return false;
			}
		}

		return true;
	};

	switch (req_type) {
		case require_type::keyword_EOF:
			if (istr.eof()) {
				throw syntax_error(syntax_error::EOF_not_found);
			}

			if (!attempt(istr, "%%EOF")) {
				throw syntax_error(syntax_error::EOF_not_found);
			}

			if (std::decay_t<decltype(istr)>::traits_type::eof() == istr.peek()) {
				return;
			}

			require(istr, require_type::EOL);
			break;
		case require_type::EOL:
			if (istr.eof()) {
				throw syntax_error(syntax_error::EOL_not_found);
			}

			if (attempt(istr, "\n")) {
				// do nothing
			} else if (attempt(istr, "\r")) {
				attempt(istr, "\n");
			} else {
				throw syntax_error(syntax_error::EOL_not_found);
			}
			break;
		case require_type::keyword_startxref:
			if (istr.eof()) {
				throw syntax_error(syntax_error::keyword_startxref_not_found);
			}

			ignore_if_present(istr,
			                  ignore_flag::any_whitespace_characters_except_EOL);
			if (!attempt(istr, "startxref")) {
				throw syntax_error(syntax_error::keyword_startxref_not_found);
			}
			ignore_if_present(istr,
			                  ignore_flag::any_whitespace_characters_except_EOL |
			                      ignore_flag::comment);
			require(istr, require_type::EOL);
			break;
		case require_type::keyword_xref:
			if (istr.eof()) {
				throw syntax_error(syntax_error::keyword_xref_not_found);
			}

			ignore_if_present(istr,
			                  ignore_flag::any_whitespace_characters_except_EOL);
			if (!attempt(istr, "xref")) {
				throw syntax_error(syntax_error::keyword_xref_not_found);
			}
			ignore_if_present(istr,
			                  ignore_flag::any_whitespace_characters_except_EOL |
			                      ignore_flag::comment);
			require(istr, require_type::EOL);
			break;
		case require_type::space:
			if (istr.eof()) {
				throw syntax_error(syntax_error::space_not_found);
			}

			if (!attempt(istr, " ")) {
				throw syntax_error(syntax_error::space_not_found);
			}
			break;
	}
}

/// <summary>
/// ignore whitespaces specified by flags if they are present on istr.
/// </summary>
/// <param name="flags">whitespace bit flags to be ignored</param>
static void ignore_if_present(std::istream& istr, ignore_flag flags) {
	assert(istr.exceptions() == (std::ios_base::badbit | std::ios_base::failbit));
	assert(!istr.fail());

	if (istr.eof()) {
		return;
	}

	using istream_t = std::istream;

	std::vector<std::function<bool(istream_t&)>> ignore_functions;
	{
		// constant (will be moved)
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
			if ((flag & flags) != ignore_flag{}) {
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

/**********************
  parser::footer Class
 **********************/
parser::footer::footer(std::istream& istr) {
	assert(istr.exceptions() == (std::ios_base::badbit | std::ios_base::failbit));
	assert(!istr.fail());

	// check %%EOF
	try {
		istr.seekg(0, std::ios_base::end); // NOTE: undefined behaviour on ISO C but
		                                   // it will go well
		seek_forward_head_of_line(istr);
	} catch (std::ios_base::failure&) {
		throw syntax_error(syntax_error::EOF_not_found);
	}
	{
		auto eof_pos = istr.tellg();
		require(istr, require_type::keyword_EOF);
		istr.seekg(eof_pos);
	}

	// get cross-reference table byte offset
	try {
		seek_forward_head_of_line(istr);
	} catch (std::ios_base::failure&) {
		throw syntax_error(syntax_error::xref_byte_offset_not_found);
	}
	{
		auto xref_byte_offset_pos = istr.tellg();
		xref_byte_offset          = take_xref_byte_offset(istr);
		istr.seekg(xref_byte_offset_pos);
	}

	// check keyword "startxref"
	try {
		seek_forward_head_of_line(istr);
	} catch (std::ios_base::failure&) {
		throw syntax_error(syntax_error::keyword_startxref_not_found);
	}
	require(istr, require_type::keyword_startxref);

	// get cross-reference table
	istr.seekg(xref_byte_offset, std::ios_base::beg);
	xref_table = take_xref_table(istr);
}
} // namespace pdfparser

#undef NOMINMAX
