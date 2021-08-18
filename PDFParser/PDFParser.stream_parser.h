#pragma once

#include "PDFParser.error_types.h"
#include "PDFParser.object_types.h"
#include "PDFParser.xref_types.h"

#include <algorithm>
#include <array>
#include <cassert>
#include <cctype>
#include <charconv>
#include <fstream>
#include <memory>
#include <sstream>

// tell test class
namespace pdfparser_test {
ref class tell_test;
} // namespace pdfparser_test

namespace pdfparser {
template <class InputStreamT>
class object_pool;

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
	constexpr object_types::dictionary_object
	    take_footer(object_pool<InputStreamT>& object_accessor);

	/// <summary>
	/// seek to end of stream
	/// </summary>
	constexpr void seek_to_end();

	/// <summary>
	/// check whether end of file or not
	/// </summary>
	/// <returns>true if and only if stream is on EOF</returns>
	constexpr bool eof();

	/// <summary>
	/// get current stream position (byte offset from beginning of file)
	/// </summary>
	/// <returns>current byte offset from beginning of file</returns>
	constexpr std::streamoff tell();

	/// <summary>
	/// move stream position to byte_offset_from_beginning_of_file
	/// </summary>
	/// <param name="byte_offset_from_beginning_of_file">
	/// byte offset from beginning of file
	/// </param>
	constexpr void seek(std::streamoff byte_offset_from_beginning_of_file);

	/// <exception cref="std::ios_base::failure">
	/// thrown when there is no beginning of line frontward
	/// </exception>
	constexpr void seek_forward_head_of_line();

	/// <summary>
	/// take byte offset of cross reference table from footer
	/// </summary>
	/// <exceptions cref="std::overflow_error">
	/// thrown when overflow against std::streamoff
	/// </exceptions>
	/// <exceptions cref="pdfparser::error_types::parse_error">
	/// thrown when invalid format detected
	/// </exceptions>
	/// <returns>byte offset of cross reference table</returns>
	constexpr std::streamoff take_xref_byte_offset();

	/// <summary>
	/// take cross reference table
	/// </summary>
	/// <exceptions cref="std::overflow_error"></exceptions>
	/// <exceptions cref="pdfparser::error_types::parse_error"></exceptions>
	/// <returns>byte offset of cross reference table</returns>
	constexpr xref_types::xref_table take_xref_table();

	/// <summary>
	/// take cross reference entry
	/// </summary>
	/// <exceptions cref="pdfparser::error_types::parse_error">
	/// thrown when invalid format was detected
	/// </exceptions>
	constexpr xref_types::xref_entry
	    take_xref_entry(xref_types::object_t object_number);

	/// <summary>
	/// get trailer dictionary
	/// </summary>
	/// <param name="object_accessor">object pool</param>
	/// <returns>trailer dictionary</returns>
	/// <exceptions cref="pdfparser::error_types::parse_error">
	/// thrown when keyword "trailer" was not found
	/// or trailer dictionary was not found
	/// </exceptions>
	constexpr object_types::dictionary_object
	    take_trailer(object_pool<InputStreamT>& object_accessor);

	/// <summary>
	/// take any direct object or indirect reference
	/// </summary>
	/// <param name="object_accessor">object pool</param>
	/// <returns>gotten object</returns>
	/// <exceptions cref="pdfparser::error_types::parse_error">
	/// thrown when any direct object or indirect reference was not taken
	/// </exceptions>
	constexpr object_types::any_direct_object_or_ref
	    take_any_object(object_pool<InputStreamT>& object_accessor);

	/// <summary>
	/// take indirect object
	/// </summary>
	/// <param name="object_accessor">object pool</param>
	/// <param name="object_number">object number to be taken</param>
	/// <param name="generation_number">generation number to be taken</param>
	/// <returns>the direct object to which this indirect object refers</returns>
	constexpr object_types::any_direct_object
	    take_indirect_object(object_pool<InputStreamT>& object_accessor,
	                         xref_types::object_t       object_number,
	                         xref_types::generation_t   generation_number);

	/// <summary>
	/// consume token attempt_str if it's present
	/// </summary>
	/// <param name="attempt_str">attempt character string</param>
	/// <returns>true if attempt_str is present</returns>
	constexpr bool attempt(std::string_view attempt_str);

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

	// test class
	friend ref class pdfparser_test::tell_test;
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

template <class InputStreamT>
constexpr object_types::dictionary_object
    stream_parser<InputStreamT>::take_footer(
        object_pool<InputStreamT>& object_accessor) {
	// check %%EOF
	seek_to_end();
	seek_forward_head_of_line();
	{
		auto eof_pos = tell();
		if (!attempt("%%EOF")) {
			throw parse_error(parse_error::keyword_EOF_not_found);
		}

		if (!(eof() || attempt("\r\n") || attempt("\n") || attempt("\r"))) {
			throw parse_error(parse_error::EOL_not_found);
		}

		seek(eof_pos);
	}

	// get cross-reference table byte offset
	seek_forward_head_of_line();
	std::streamoff xref_byte_offset;
	{
		auto xref_byte_offset_pos = tell();
		xref_byte_offset          = take_xref_byte_offset();
		seek(xref_byte_offset_pos);
	}

	// check keyword "startxref"
	seek_forward_head_of_line();
	ignore_if_present(ignore_flag::any_whitespace_characters_except_EOL);
	if (!attempt("startxref")) {
		throw parse_error(parse_error::keyword_startxref_not_found);
	}
	ignore_if_present(ignore_flag::any_whitespace_characters_except_EOL |
	                  ignore_flag::comment);
	if (!(attempt("\r\n") || attempt("\n") || attempt("\r"))) {
		throw parse_error(parse_error::EOL_not_found);
	}

	// get cross-reference table
	seek(xref_byte_offset);
	auto xref_table = take_xref_table();

	ignore_if_present(ignore_flag::any_whitespace_characters |
	                  ignore_flag::comment);

	// get trailer dictionary
	object_accessor.add_xref_table(xref_table);
	return take_trailer(object_accessor);
}

template <class InputStreamT>
constexpr void stream_parser<InputStreamT>::seek_to_end() {
	// NOTE: undefined behaviour on ISO C but it will go well on Windows
	m_stream.seekg(0, std::ios_base::end);
}

template <class InputStreamT>
constexpr bool stream_parser<InputStreamT>::eof() {
	return m_stream.eof() ||
	       decltype(m_stream)::traits_type::eof() == m_stream.peek();
}

template <class InputStreamT>
constexpr std::streamoff stream_parser<InputStreamT>::tell() {
	m_stream.clear(m_stream.rdstate() & ~std::ios_base::eofbit);
	return m_stream.tellg();
}

template <class InputStreamT>
constexpr void stream_parser<InputStreamT>::seek(
    std::streamoff byte_offset_from_beginning_of_file) {
	try {
		m_stream.seekg(byte_offset_from_beginning_of_file, std::ios_base::beg);
	} catch (std::ios_base::failure&) {
		throw parse_error(parse_error::failed_to_seek);
	}
}

template <class InputStreamT>
constexpr void stream_parser<InputStreamT>::seek_forward_head_of_line() {
	// immediately preceding newline character
	try {
		m_stream.seekg(-1, std::ios_base::cur);
	} catch (std::ios_base::failure&) {
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

	if (!(attempt("\r\n") || attempt("\n") || attempt("\r"))) {
		throw parse_error(parse_error::EOL_not_found);
	}

	return xref_byte_offset;
}

template <class InputStreamT>
constexpr xref_types::xref_table
    stream_parser<InputStreamT>::take_xref_table() {
	using namespace xref_types;

	xref_table this_xref_table;

	ignore_if_present(ignore_flag::any_whitespace_characters_except_EOL);
	if (!attempt("xref")) {
		throw parse_error(parse_error::keyword_xref_not_found);
	}
	ignore_if_present(ignore_flag::any_whitespace_characters_except_EOL |
	                  ignore_flag::comment);
	if (!(attempt("\r\n") || attempt("\n") || attempt("\r"))) {
		throw parse_error(parse_error::EOL_not_found);
	}

	const object_t first_object_number = take_unsigned_integer<object_t>();
	if (!attempt(" ")) {
		throw parse_error(parse_error::space_not_found);
	}
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
	if (!(attempt("\r\n") || attempt("\n") || attempt("\r"))) {
		throw parse_error(parse_error::EOL_not_found);
	}

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

	if (!attempt(" ")) {
		throw parse_error(parse_error::space_not_found);
	}

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

	if (!attempt(" ")) {
		throw parse_error(parse_error::space_not_found);
	}

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

template <class InputStreamT>
constexpr object_types::dictionary_object
    stream_parser<InputStreamT>::take_trailer(
        object_pool<InputStreamT>& object_accessor) {
	if (!attempt("trailer")) {
		throw parse_error(parse_error::keyword_trailer_not_found);
	}
	ignore_if_present(ignore_flag::any_whitespace_characters |
	                  ignore_flag::comment);

	auto trailer_dictionary = take_any_object(object_accessor);

	if (!std::holds_alternative<object_types::dictionary_object>(
	        trailer_dictionary)) {
		throw parse_error(parse_error::trailer_dictionary_not_found);
	}
	return std::get<object_types::dictionary_object>(trailer_dictionary);
}

template <class InputStreamT>
constexpr object_types::any_direct_object_or_ref
    stream_parser<InputStreamT>::take_any_object(
        object_pool<InputStreamT>& object_accessor) {
	using namespace object_types;

	if (eof()) {
		throw parse_error(parse_error::object_not_found);
	} else if (attempt("true")) {
		return boolean_object(true);
	} else if (attempt("false")) {
		return boolean_object(false);
	} else if (attempt("null")) {
		return null;
	} else if (attempt("(")) {
		string_object literal_string;
		// NOTE: at least size_t seems to be enough.
		std::size_t number_of_left_parenthesis  = 1;
		std::size_t number_of_right_parenthesis = 0;
		while (!eof() &&
		       number_of_left_parenthesis != number_of_right_parenthesis) {
			// assert: noexcept
			switch (const auto ch = m_stream.get()) {
			case '(':
				++number_of_left_parenthesis;
				literal_string.push_back('(');
				break;
			case ')':
				++number_of_right_parenthesis;
				if (number_of_left_parenthesis != number_of_right_parenthesis) {
					literal_string.push_back(')');
				}
				break;
			case '\r':
				literal_string.push_back('\n');
				attempt("\n");
				break;
			case '\\':
				if (eof()) {
					break;
				}

				// assert: noexcept
				switch (const auto ch_after_backslash = m_stream.get()) {
				case 'n':
					literal_string.push_back('\n');
					break;
				case 'r':
					literal_string.push_back('\r');
					break;
				case 't':
					literal_string.push_back('\t');
					break;
				case 'b':
					literal_string.push_back('\b');
					break;
				case 'f':
					literal_string.push_back('\f');
					break;
				case '(':
					literal_string.push_back('(');
					break;
				case ')':
					literal_string.push_back(')');
					break;
				case '\\':
					literal_string.push_back('\\');
					break;
				case '\r': // backslash + EOL -> delete
					attempt("\n");
					break;
				case '\n':
					// do nothing
					break;
				default:
					if ('0' <= ch_after_backslash && ch_after_backslash <= '7') {
						// octal
						uint8_t octal_character =
						    static_cast<uint8_t>(ch_after_backslash - '0');
						if (!eof()) {
							if (const auto second_digit = m_stream.get();
							    '0' <= second_digit && second_digit <= '7') {
								octal_character = static_cast<decltype(octal_character)>(
								    8 * octal_character + (second_digit - '0'));
								if (!eof()) {
									if (const auto third_digit = m_stream.get();
									    '0' <= third_digit && third_digit <= '7') {
										octal_character = static_cast<decltype(octal_character)>(
										    8 * octal_character + (third_digit - '0'));
									}
								}
							}
						}
						literal_string.push_back(static_cast<char>(octal_character));
					} else {
						literal_string.push_back(static_cast<char>(ch_after_backslash));
					}
					break;
				}
				break;
			default:
				literal_string.push_back(static_cast<char>(ch));
				break;
			}
		}

		if (number_of_left_parenthesis != number_of_right_parenthesis) {
			throw parse_error(parse_error::literal_string_lack_of_right_parenthesis);
		}

		return literal_string;
	} else if (attempt("/")) {
		name_object    name;
		constexpr auto whitespace_list = {'\0', '\t', '\n', '\f', '\r', ' ', '%'};
		while (!eof() && std::find(whitespace_list.begin(), whitespace_list.end(),
		                           m_stream.peek()) == whitespace_list.end()) {
			if (const char ch = static_cast<char>(m_stream.get()); '#' == ch) {
				char digits[3] = {};
				if (2 == m_stream.readsome(digits, 2) && std::isxdigit(digits[0]) &&
				    std::isxdigit(digits[1])) {
					name.push_back(
					    static_cast<unsigned char>(std::strtol(digits, nullptr, 16)));
				} else {
					throw parse_error(parse_error::name_invalid_hexadecimal_code);
				}
			} else {
				name.push_back(ch);
			}
		}
		return name;
	} else if (attempt("[")) {
		array_object array;

		ignore_if_present(ignore_flag::any_whitespace_characters |
		                  ignore_flag::comment);
		while (!attempt("]")) {
			if (eof()) {
				throw parse_error(parse_error::array_lack_of_right_square_bracket);
			}
			array.push_back(take_any_object(object_accessor));
			ignore_if_present(ignore_flag::any_whitespace_characters |
			                  ignore_flag::comment);
		}

		return array;
	} else if (attempt("<<")) {
		dictionary_object dictionary;

		ignore_if_present(ignore_flag::any_whitespace_characters |
		                  ignore_flag::comment);
		while (!attempt(">>")) {
			if (eof()) {
				throw parse_error(
				    parse_error::dictionary_lack_of_double_greater_than_sign);
			}

			auto name = take_any_object(object_accessor);
			if (!std::holds_alternative<name_object>(name)) {
				throw parse_error(parse_error::dictionary_key_is_not_name_object);
			}

			ignore_if_present(ignore_flag::any_whitespace_characters |
			                  ignore_flag::comment);
			auto value = take_any_object(object_accessor);

			// emplace to dictionary
			std::visit(
			    [&dictionary, &name](const auto& concrete_value) {
				    using T = std::decay_t<decltype(concrete_value)>;
				    if constexpr (!std::is_same_v<null_object, T>) {
					    dictionary.emplace(std::get<name_object>(name), concrete_value);
				    }
			    },
			    value);

			ignore_if_present(ignore_flag::any_whitespace_characters |
			                  ignore_flag::comment);
		}

		ignore_if_present(ignore_flag::any_whitespace_characters |
		                  ignore_flag::comment);

		// determined to be Dictionary Object
		if (!attempt("stream")) {
			return dictionary;
		}

		// determined to be Stream Object
		if (!(attempt("\r\n") || attempt("\n"))) {
			throw parse_error(parse_error::stream_CRLF_or_LF_not_found);
		}

		const auto length_it = dictionary.find("Length");
		if (length_it == dictionary.end()) {
			throw parse_error(parse_error::stream_dictionary_absence_of_Length_entry);
		}

		const auto stream_length_var = std::visit(
		    [&object_accessor](const auto& concrete_object) {
			    return object_accessor.dereference(concrete_object);
		    },
		    length_it->second);
		const auto stream_length_p =
		    std::get_if<integer_object>(&stream_length_var);
		if (nullptr == stream_length_p) {
			throw parse_error(parse_error::stream_dictionary_Length_is_not_Integer);
		}
		const auto stream_length = *stream_length_p;

		auto stream_data = std::make_unique<char[]>(stream_length);
		// HACK: stream_length
		// バイト読み取り中に、Filterに対する明示的なEODマーカーが出現した場合にエラーにする
		if (m_stream.readsome(stream_data.get(), stream_length) != stream_length) {
			throw parse_error(parse_error::stream_data_is_shorter_than_Length);
		}
		// at least one EOL is required
		if (!(attempt("\r\n") || attempt("\n") || attempt("\r"))) {
			throw parse_error(parse_error::EOL_not_found);
		}

		// additional EOLs are permitted
		ignore_if_present(ignore_flag::EOL);

		if (!attempt("endstream")) {
			throw parse_error(parse_error::keyword_endstream_not_found);
		}

		return stream_object{std::move(dictionary),
		                     std::string(stream_data.get(), stream_length)};
	} else if (attempt("<")) {
		string_object hexadecimal_string;

		unsigned char character     = 0;
		int           hex_digit_pos = 0;
		ignore_if_present(ignore_flag::any_whitespace_characters |
		                  ignore_flag::comment);
		while (!attempt(">")) {
			if (eof()) {
				throw parse_error(
				    parse_error::hexadecimal_string_lack_of_greater_than_sign);
			}
			++hex_digit_pos;
			assert(1 == hex_digit_pos || 2 == hex_digit_pos);

			char hex_digit = static_cast<char>(m_stream.get());
			if (!std::isxdigit(hex_digit)) {
				throw parse_error(
				    parse_error::hexadecimal_string_non_hexadecimal_digit_found);
			}

			character |= std::stoi(std::string(1, hex_digit), nullptr, 16)
			             << (4 * (2 - hex_digit_pos));

			if (2 == hex_digit_pos) {
				hexadecimal_string.push_back(character);
				character     = 0;
				hex_digit_pos = 0;
			}

			ignore_if_present(ignore_flag::any_whitespace_characters |
			                  ignore_flag::comment);
		}
		if (1 == hex_digit_pos) {
			hexadecimal_string.push_back(character);
		}

		return hexadecimal_string;
	} else if (std::isdigit(m_stream.peek()) || '.' == m_stream.peek() ||
	           '+' == m_stream.peek() || '-' == m_stream.peek()) {
		bool plus = true;

		// dealing with sign
		if ('+' == m_stream.peek()) {
			plus = true;
			m_stream.seekg(1, std::ios_base::cur);
		} else if ('-' == m_stream.peek()) {
			plus = false;
			m_stream.seekg(1, std::ios_base::cur);
		}

		std::string integer_part = plus ? "+" : "-";
		int         last_ch      = 0;
		while (!eof() && std::isdigit(last_ch = m_stream.peek())) {
			integer_part.push_back(static_cast<char>(m_stream.get()));
		}

		if ('.' == last_ch) {
			// determined to be Real Object

			// skip '.'
			// assert: noexcept
			m_stream.seekg(1, std::ios_base::cur);

			std::string fractional_part;
			while (!eof() && std::isdigit(m_stream.peek())) {
				fractional_part.push_back(static_cast<char>(m_stream.get()));
			}

			if (integer_part.length() == 0 && fractional_part.length() == 0) {
				throw parse_error(parse_error::unknown_character_detected);
			}

			return real_object{std::stod(integer_part + '.' + fractional_part)};
		}

		const auto after_first_integer_byte_offset = tell();

		// determined not to be Real Object
		const auto first_integer_str = integer_part;

		ignore_if_present(ignore_flag::any_whitespace_characters |
		                  ignore_flag::comment);
		xref_types::generation_t generation_number;
		try {
			generation_number = take_signed_integer<xref_types::generation_t>();
		} catch (const parse_error& parse_e) {
			if (parse_error::signed_integer_not_found == parse_e.code()) {
				// determined to be Integer Object

				int integer;
				try {
					std::istringstream istr{first_integer_str};
					istr >> integer;
				} catch (std::ios_base::failure&) {
					throw std::overflow_error("overflow");
				}

				// restore stream position
				seek(after_first_integer_byte_offset);

				return integer_object{integer};
			} else {
				// rethrow
				throw;
			}
		}

		ignore_if_present(ignore_flag::any_whitespace_characters |
		                  ignore_flag::comment);

		if (attempt("R")) {
			// determined to be Indirect Reference

			xref_types::object_t object_number;
			try {
				std::istringstream istr{first_integer_str};
				istr >> object_number;
			} catch (std::ios_base::failure&) {
				throw std::overflow_error("overflow");
			}
			return indirect_reference{object_number, generation_number};
		}

		// determined to be Integer Object
		int integer;
		try {
			std::istringstream istr{first_integer_str};
			istr >> integer;
		} catch (std::ios_base::failure&) { throw std::overflow_error("overflow"); }

		// restore stream position
		seek(after_first_integer_byte_offset);

		return integer_object{integer};
	} else {
		throw parse_error(parse_error::unknown_character_detected);
	}
}

template <class InputStreamT>
constexpr object_types::any_direct_object
    stream_parser<InputStreamT>::take_indirect_object(
        object_pool<InputStreamT>& object_accessor,
        xref_types::object_t       object_number,
        xref_types::generation_t   generation_number) {
	xref_types::object_t first_integer =
	    take_signed_integer<xref_types::object_t>();
	ignore_if_present(ignore_flag::any_whitespace_characters |
	                  ignore_flag::comment);
	xref_types::generation_t second_integer =
	    take_signed_integer<xref_types::generation_t>();
	if (first_integer != object_number || second_integer != generation_number) {
		throw parse_error(
		    parse_error::indirect_object_is_inconsistent_with_xref_table);
	}

	ignore_if_present(ignore_flag::any_whitespace_characters |
	                  ignore_flag::comment);
	if (!attempt("obj")) {
		throw parse_error(parse_error::keyword_obj_not_found);
	}

	ignore_if_present(ignore_flag::any_whitespace_characters |
	                  ignore_flag::comment);
	auto object          = take_any_object(object_accessor);
	auto concrete_object = std::visit(
	    [](const auto& concrete_object) -> object_types::any_direct_object {
		    using T = std::decay_t<decltype(concrete_object)>;
		    if constexpr (std::is_same_v<object_types::indirect_reference, T>) {
			    throw parse_error(
			        parse_error::indirect_object_refers_indirect_reference);
		    } else {
			    return concrete_object;
		    }
	    },
	    object);

	ignore_if_present(ignore_flag::any_whitespace_characters |
	                  ignore_flag::comment);
	if (!attempt("endobj")) {
		throw parse_error(parse_error::keyword_endobj_not_found);
	}

	return concrete_object;
}

template <class InputStreamT>
constexpr bool
    stream_parser<InputStreamT>::attempt(std::string_view attempt_str) {
	m_stream.clear(m_stream.rdstate() & ~std::ios_base::eofbit);

	const auto old_pos = m_stream.tellg();
	for (auto attempt_char : attempt_str) {
		if (static_cast<decltype(m_stream)::int_type>(attempt_char) ==
		    m_stream.peek()) {
			m_stream.seekg(1, std::ios_base::cur);
		} else {
			m_stream.seekg(old_pos);
			return false;
		}
	}

	return true;
}

template <class InputStreamT>
constexpr void
    stream_parser<InputStreamT>::ignore_if_present(ignore_flag flags) {
	if (eof()) {
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
		         {ignore_flag::comment, [this](istream_t& m_stream) {
			          if (m_stream.peek() != '%') {
				          return false;
			          }
			          m_stream.seekg(1, std::ios_base::cur);

			          // skip comment contents
			          while (!eof() && m_stream.peek() != '\r' &&
			                 m_stream.peek() != '\n') {
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
	while (ignored && !eof()) {
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
	if (eof()) {
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
	if (eof() || !std::isdigit(static_cast<unsigned char>(m_stream.peek()))) {
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
	if (eof()) {
		throw parse_error(parse_error::unsigned_integer_not_found);
	}

	// if m_stream does not begin with any of "0123456789"
	if (eof() || !std::isdigit(static_cast<unsigned char>(m_stream.peek()))) {
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
