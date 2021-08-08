/*
#include "PDFParser.error_types.cpp"
#include "PDFParser.parser.cpp"
#include "PDFParser.xref_types.cpp"
*/

#include "PDFParser.h"

#include <iomanip>
#include <limits>
#include <sstream>

using namespace pdfparser;
using namespace pdfparser::error_types;

#pragma region seek_forward_head_of_line_test
#include "seek_forward_head_of_line_test.hpp"
using namespace pdfparser_test;
/**
 * | abc
 * def の時にエラーになるか
 */
void seek_forward_head_of_line_test::test_beginning_of_file() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);
	stream << "abc\ndef";

	stream_parser str_parser(std::move(stream));
	try {
		str_parser.seek_forward_head_of_line();
	} catch (parse_error& parse_e) {
		Assert::IsTrue(
		    parse_error::error_code::failed_to_seek_forward_head_of_line ==
		    parse_e.code());

		// success
		return;
	}
	Assert::Fail();
}
/**
 * abc
 * def
 * | ghi の時に
 *
 * abc
 * | def
 * ghi になるか
 */
void seek_forward_head_of_line_test::test_beginning_of_line() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "abc\n";
	std::streamoff expected = stream.tellp();

	stream << "def\n";
	auto head_of_ghi = stream.tellp();

	stream << "ghi\n";
	stream.seekg(head_of_ghi);

	stream_parser str_parser(std::move(stream));
	str_parser.seek_forward_head_of_line();
	Assert::IsTrue(expected == str_parser.tell());
}
/**
 * abc
 * de | f の時に
 *
 * abc
 * | def になるか
 */
void seek_forward_head_of_line_test::test_middle_of_line() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "abc\n";
	std::streamoff expected = stream.tellp();

	stream << "de";
	auto middle_of_def = stream.tellp();

	stream << "f\n";
	stream.seekg(middle_of_def);

	stream_parser str_parser(std::move(stream));
	str_parser.seek_forward_head_of_line();
	Assert::IsTrue(expected == str_parser.tell());
}
/**
 * CR | の時に
 * | CR になるか
 */
void seek_forward_head_of_line_test::test_CR() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	std::streamoff expected = stream.tellp();

	stream << "\r";
	stream.seekg(stream.tellp());

	stream_parser str_parser(std::move(stream));
	str_parser.seek_forward_head_of_line();
	Assert::IsTrue(expected == str_parser.tell());
}
/**
 * LF | の時に
 * | LF になるか
 */
void seek_forward_head_of_line_test::test_LF() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	std::streamoff expected = stream.tellp();

	stream << "\n";
	stream.seekg(stream.tellp());

	stream_parser str_parser(std::move(stream));
	str_parser.seek_forward_head_of_line();
	Assert::IsTrue(expected == str_parser.tell());
}
/**
 * CRLF | の時に
 * | CRLF になるか
 */
void seek_forward_head_of_line_test::test_CRLF() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	std::streamoff expected = stream.tellp();

	stream << "\r\n";
	stream.seekg(stream.tellp());

	stream_parser str_parser(std::move(stream));
	str_parser.seek_forward_head_of_line();
	Assert::IsTrue(expected == str_parser.tell());
}
/**
 * c CR CR | の時に
 * c CR | CR になるか
 */
void seek_forward_head_of_line_test::test_CR_CRCR() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "c\r";
	std::streamoff expected = stream.tellp();

	stream << "\r";
	stream.seekg(stream.tellp());

	stream_parser str_parser(std::move(stream));
	str_parser.seek_forward_head_of_line();
	Assert::IsTrue(expected == str_parser.tell());
}
/**
 * c LF CR | の時に
 * c LF | CR になるか
 */
void seek_forward_head_of_line_test::test_CR_LFCR() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "c\n";
	std::streamoff expected = stream.tellp();

	stream << "\r";
	stream.seekg(stream.tellp());

	stream_parser str_parser(std::move(stream));
	str_parser.seek_forward_head_of_line();
	Assert::IsTrue(expected == str_parser.tell());
}
/**
 * c LF LF | の時に
 * c LF | LF になるか
 */
void seek_forward_head_of_line_test::test_LF_LFLF() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "c\n";
	std::streamoff expected = stream.tellp();

	stream << "\n";
	stream.seekg(stream.tellp());

	stream_parser str_parser(std::move(stream));
	str_parser.seek_forward_head_of_line();
	Assert::IsTrue(expected == str_parser.tell());
}
/**
 * c CR CRLF | の時に
 * c CR | CRLF になるか
 */
void seek_forward_head_of_line_test::test_CRLF_CRCRLF() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "c\r";
	std::streamoff expected = stream.tellp();

	stream << "\r\n";
	stream.seekg(stream.tellp());

	stream_parser str_parser(std::move(stream));
	str_parser.seek_forward_head_of_line();
	Assert::IsTrue(expected == str_parser.tell());
}
/**
 * c LF CRLF | の時に
 * c LF | CRLF になるか
 */
void seek_forward_head_of_line_test::test_CRLF_LFCRLF() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "c\n";
	std::streamoff expected = stream.tellp();

	stream << "\r\n";
	stream.seekg(stream.tellp());

	stream_parser str_parser(std::move(stream));
	str_parser.seek_forward_head_of_line();
	Assert::IsTrue(expected == str_parser.tell());
}
#pragma endregion // region seek_forward_head_of_line_test

#pragma region take_xref_table_test
#include "take_xref_table_test.hpp"
using namespace pdfparser_test;
// test largest xref table
void take_xref_table_test::test_maximum_xref_table() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "xref\n";
	stream << "1 " << std::numeric_limits<xref_types::object_t>::max() << "\n";

	stream_parser str_parser(std::move(stream));
	try {
		str_parser.take_xref_table();
	} catch (const parse_error& parse_e) {
		Assert::IsTrue(parse_error::xref_entry_first_10_digits_invalid ==
		               parse_e.code());

		// success
		return;
	}
	Assert::Fail();
}
// test too large xref table (which occurs std::overflow_error)
void take_xref_table_test::test_overflow() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "xref\n";
	constexpr auto max = std::numeric_limits<xref_types::object_t>::max();
	stream << "1 " << (max / 10) << (max % 10 + 1) << "\n";

	stream_parser str_parser(std::move(stream));
	try {
		str_parser.take_xref_table();
	} catch (std::overflow_error&) {
		// success
		return;
	}
	Assert::Fail();
}
#pragma endregion // region take_xref_table_test

#pragma region take_xref_entry_test
#include "take_xref_entry_test.hpp"
using namespace pdfparser_test;
void take_xref_entry_test::test_SPCR() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	using namespace xref_types;

	constexpr object_t     object_number = 1;
	constexpr auto         next_free     = std::numeric_limits<object_t>::max();
	constexpr generation_t generation_number =
	    std::numeric_limits<generation_t>::max();
	stream << std::setfill('0');
	stream << std::setw(10) << next_free << " " << std::setw(5)
	       << generation_number;
	stream << " f";
	stream << " \r";
	assert(stream.str().length() == 20);

	stream_parser str_parser(std::move(stream));
	Assert::IsTrue(
	    str_parser.take_xref_entry(object_number) ==
	    xref_entry{xref_free_entry{object_number, generation_number, next_free}});
}
void take_xref_entry_test::test_SPLF() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	using namespace xref_types;

	constexpr object_t     object_number = 0;
	constexpr auto         next_free     = 0;
	constexpr generation_t generation_number =
	    std::numeric_limits<generation_t>::max();
	stream << std::setfill('0');
	stream << std::setw(10) << next_free << " " << std::setw(5)
	       << generation_number;
	stream << " f";
	stream << " \n";
	assert(stream.str().length() == 20);

	stream_parser str_parser(std::move(stream));
	Assert::IsTrue(
	    str_parser.take_xref_entry(object_number) ==
	    xref_entry{xref_free_entry{object_number, generation_number, next_free}});
}
void take_xref_entry_test::test_CRLF() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	using namespace xref_types;

	constexpr object_t     object_number = std::numeric_limits<object_t>::max();
	constexpr auto         byte_offset   = 9999999999;
	constexpr generation_t generation_number =
	    std::numeric_limits<generation_t>::max();
	stream << std::setfill('0');
	stream << std::setw(10) << byte_offset << " " << std::setw(5)
	       << generation_number;
	stream << " n";
	stream << "\r\n";
	assert(stream.str().length() == 20);

	stream_parser str_parser(std::move(stream));
	Assert::IsTrue(str_parser.take_xref_entry(object_number) ==
	               xref_entry{xref_inuse_entry{object_number, generation_number,
	                                           byte_offset}});
}
#pragma endregion // take_xref_entry_test

#pragma region ignore_if_present_test
#include "ignore_if_present_test.hpp"
using namespace pdfparser_test;
void ignore_if_present_test::test_null_only() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << '\0' << " \t\n\f\r ";

	stream_parser str_parser(std::move(stream));
	str_parser.ignore_if_present(ignore_flag::null);
	Assert::IsTrue(1 == str_parser.tell());
}
void ignore_if_present_test::test_line_feed_only() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "\n\r";

	stream_parser str_parser(std::move(stream));
	str_parser.ignore_if_present(ignore_flag::line_feed);
	Assert::IsTrue(1 == str_parser.tell());
}
void ignore_if_present_test::test_form_feed_only() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << '\f' << " \t\n\f\r ";

	stream_parser str_parser(std::move(stream));
	str_parser.ignore_if_present(ignore_flag::form_feed);
	Assert::IsTrue(1 == str_parser.tell());
}
void ignore_if_present_test::test_carriage_return_only() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "\r\n";

	stream_parser str_parser(std::move(stream));
	str_parser.ignore_if_present(ignore_flag::carriage_return);
	Assert::IsTrue(1 == str_parser.tell());
}
void ignore_if_present_test::test_space_only() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << ' ' << "\t\n\f\r ";

	stream_parser str_parser(std::move(stream));
	str_parser.ignore_if_present(ignore_flag::space);
	Assert::IsTrue(1 == str_parser.tell());
}
void ignore_if_present_test::test_comment_only() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "% this is comment section";
	std::streamoff expected = stream.tellp();

	stream << "\n\r ";

	stream_parser str_parser(std::move(stream));
	str_parser.ignore_if_present(ignore_flag::comment);
	Assert::IsTrue(expected == str_parser.tell());
}
void ignore_if_present_test::test_EOL_only() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "\r\n ";

	stream_parser str_parser(std::move(stream));
	str_parser.ignore_if_present(ignore_flag::EOL);
	Assert::IsTrue(2 == str_parser.tell());
}
void ignore_if_present_test::test_any_whitespace_characters() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << '\0' << "\t\n\f\r ";
	std::streamoff expected = stream.tellp();

	stream_parser str_parser(std::move(stream));
	str_parser.ignore_if_present(ignore_flag::any_whitespace_characters);
	Assert::IsTrue(expected == str_parser.tell());
}
void ignore_if_present_test::test_all_whitespaces_including_comment() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << '\0' << "\t\n\f\r %comment section\r\n"
	       << "\t\n\r\n " << '\0' << "\r\n %comment 2 section!\r"
	       << "% last comment !\n ";
	std::streamoff expected = stream.tellp();

	stream_parser str_parser(std::move(stream));
	str_parser.ignore_if_present(ignore_flag::any_whitespace_characters |
	                             ignore_flag::comment);
	Assert::IsTrue(expected == str_parser.tell());
}
void ignore_if_present_test::test_nothing_to_ignore() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "don't ignore\r\n";

	stream_parser str_parser(std::move(stream));
	str_parser.ignore_if_present(ignore_flag::any_whitespace_characters |
	                             ignore_flag::comment);
	Assert::IsTrue(0 == str_parser.tell());
}
void ignore_if_present_test::test_no_flags() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << '\0' << "\t\n\f\r ";

	stream_parser str_parser(std::move(stream));
	str_parser.ignore_if_present(ignore_flag::null & ~ignore_flag::null);
	Assert::IsTrue(0 == str_parser.tell());
}
#pragma endregion // region ignore_if_present_test

#pragma region take_signed_integer_test
#include "take_signed_integer_test.hpp"
using namespace pdfparser_test;
void take_signed_integer_test::test_unsigned() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "100";
	std::streamoff expected = stream.tellp();

	stream_parser str_parser(std::move(stream));
	auto          integer = str_parser.take_signed_integer<long long>();
	Assert::AreEqual((long long)100, integer);
	Assert::IsTrue(expected == str_parser.tell());
}
void take_signed_integer_test::test_plus() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "+100";
	std::streamoff expected = stream.tellp();

	stream_parser str_parser(std::move(stream));
	auto          integer = str_parser.take_signed_integer<long long>();
	Assert::AreEqual((long long)100, integer);
	Assert::IsTrue(expected == str_parser.tell());
}
void take_signed_integer_test::test_minus() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "-100";
	std::streamoff expected = stream.tellp();

	stream_parser str_parser(std::move(stream));
	auto          integer = str_parser.take_signed_integer<long long>();
	Assert::AreEqual((long long)-100, integer);
	Assert::IsTrue(expected == str_parser.tell());
}
void take_signed_integer_test::test_max() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	constexpr auto max = std::numeric_limits<long long>::max();
	stream << max;
	std::streamoff expected = stream.tellp();

	stream_parser str_parser(std::move(stream));
	auto          integer = str_parser.take_signed_integer<long long>();
	Assert::AreEqual(max, integer);
	Assert::IsTrue(expected == str_parser.tell());
}
void take_signed_integer_test::test_min() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	constexpr auto min = std::numeric_limits<long long>::min();
	stream << min;
	std::streamoff expected = stream.tellp();

	stream_parser str_parser(std::move(stream));
	auto          integer = str_parser.take_signed_integer<long long>();
	Assert::AreEqual(min, integer);
	Assert::IsTrue(expected == str_parser.tell());
}

void take_signed_integer_test::test_not_number() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "ABC";

	stream_parser str_parser(std::move(stream));
	try {
		str_parser.take_signed_integer<long long>();
	} catch (const parse_error& parse_e) {
		Assert::IsTrue(parse_error::signed_integer_not_found == parse_e.code());

		// success
		return;
	}
	Assert::Fail();
}
void take_signed_integer_test::test_EOF() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream_parser str_parser(std::move(stream));
	try {
		str_parser.take_signed_integer<long long>();
	} catch (const parse_error& parse_e) {
		Assert::IsTrue(parse_error::signed_integer_not_found == parse_e.code());

		// success
		return;
	}
	Assert::Fail();
}
void take_signed_integer_test::test_plus_not_number() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "+deadbeef";

	stream_parser str_parser(std::move(stream));
	try {
		str_parser.take_signed_integer<long long>();
	} catch (const parse_error& parse_e) {
		Assert::IsTrue(parse_error::signed_integer_not_found == parse_e.code());

		// success
		return;
	}
	Assert::Fail();
}
void take_signed_integer_test::test_minus_not_number() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "-deadbeef";

	stream_parser str_parser(std::move(stream));
	try {
		str_parser.take_signed_integer<long long>();
	} catch (const parse_error& parse_e) {
		Assert::IsTrue(parse_error::signed_integer_not_found == parse_e.code());

		// success
		return;
	}
	Assert::Fail();
}
void take_signed_integer_test::test_sign_EOF() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "+";

	stream_parser str_parser(std::move(stream));
	try {
		str_parser.take_signed_integer<long long>();
	} catch (const parse_error& parse_e) {
		Assert::IsTrue(parse_error::signed_integer_not_found == parse_e.code());

		// success
		return;
	}
	Assert::Fail();
}
void take_signed_integer_test::test_max_plus1() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	constexpr auto max = std::numeric_limits<long long>::max();
	stream << max / 10 << ((max % 10) + 1);

	stream_parser str_parser(std::move(stream));
	try {
		str_parser.take_signed_integer<long long>();
	} catch (std::overflow_error&) {
		// success
		return;
	}
	Assert::Fail();
}
void take_signed_integer_test::test_min_minus1() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	constexpr auto min = std::numeric_limits<long long>::min();
	stream << min / 10 << (-(min % 10) + 1);

	stream_parser str_parser(std::move(stream));
	try {
		str_parser.take_signed_integer<long long>();
	} catch (std::overflow_error&) {
		// success
		return;
	}
	Assert::Fail();
}
#pragma endregion // region take_signed_integer_test

#pragma region take_unsigned_integer_test
#include "take_unsigned_integer_test.hpp"
using namespace pdfparser_test;
void take_unsigned_integer_test::test_unsigned() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "17";
	std::streamoff expected = stream.tellp();

	stream_parser str_parser(std::move(stream));
	auto          integer = str_parser.take_unsigned_integer<long long>();
	Assert::AreEqual((long long)17, integer);
	Assert::IsTrue(expected == str_parser.tell());
}
void take_unsigned_integer_test::test_max() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	constexpr auto max = std::numeric_limits<unsigned long long>::max();
	stream << max;
	std::streamoff expected = stream.tellp();

	stream_parser str_parser(std::move(stream));
	auto integer = str_parser.take_unsigned_integer<unsigned long long>();
	Assert::AreEqual(max, integer);
	Assert::IsTrue(expected == str_parser.tell());
}
void take_unsigned_integer_test::test_min() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << 0;
	std::streamoff expected = stream.tellp();

	stream_parser str_parser(std::move(stream));
	auto integer = str_parser.take_unsigned_integer<unsigned long long>();
	Assert::AreEqual((unsigned long long)0, integer);
	Assert::IsTrue(expected == str_parser.tell());
}

void take_unsigned_integer_test::test_not_number() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "deadbeef";

	stream_parser str_parser(std::move(stream));
	try {
		str_parser.take_unsigned_integer<long long>();
	} catch (const parse_error& parse_e) {
		Assert::IsTrue(parse_error::unsigned_integer_not_found == parse_e.code());

		// success
		return;
	}
	Assert::Fail();
}
void take_unsigned_integer_test::test_EOF() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream_parser str_parser(std::move(stream));
	try {
		str_parser.take_unsigned_integer<long long>();
	} catch (const parse_error& parse_e) {
		Assert::IsTrue(parse_error::unsigned_integer_not_found == parse_e.code());

		// success
		return;
	}
	Assert::Fail();
}
void take_unsigned_integer_test::test_signed() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "+17";

	stream_parser str_parser(std::move(stream));
	try {
		str_parser.take_unsigned_integer<long long>();
	} catch (const parse_error& parse_e) {
		Assert::IsTrue(parse_error::unsigned_integer_not_found == parse_e.code());

		// success
		return;
	}
	Assert::Fail();
}
void take_unsigned_integer_test::test_max_plus1() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	constexpr auto max = std::numeric_limits<unsigned long long>::max();
	stream << (max / 10) << (max % 10 + 1);

	stream_parser str_parser(std::move(stream));
	try {
		str_parser.take_unsigned_integer<long long>();
	} catch (std::overflow_error&) {
		// success
		return;
	}
	Assert::Fail();
}
#pragma endregion // region take_unsigned_integer_test

#pragma region require_test
#include "require_test.hpp"
using namespace pdfparser_test;
void require_test::test_EOF_EOF_EOL() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "%%EOF\r\n";
	std::streamoff expected = stream.tellp();

	stream_parser str_parser(std::move(stream));
	str_parser.require(require_type::keyword_EOF);
	Assert::IsTrue(expected == str_parser.tell());
}
void require_test::test_EOF_EOF_only() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "%%EOF";
	std::streamoff expected = stream.tellp();

	stream_parser str_parser(std::move(stream));
	str_parser.require(require_type::keyword_EOF);
	Assert::IsTrue(expected == str_parser.tell());
}
void require_test::test_EOF_EOF_not_EOL() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "%%EOF ";

	stream_parser str_parser(std::move(stream));
	try {
		str_parser.require(require_type::keyword_EOF);
	} catch (const error_types::parse_error& e) {
		Assert::IsTrue(parse_error::EOL_not_found == e.code());

		// success
		return;
	}
	Assert::Fail();
}
void require_test::test_EOF_not_EOF() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "%EOF\n";

	stream_parser str_parser(std::move(stream));
	try {
		str_parser.require(require_type::keyword_EOF);
	} catch (const error_types::parse_error& e) {
		Assert::IsTrue(parse_error::keyword_EOF_not_found == e.code());

		// success
		return;
	}
	Assert::Fail();
}
void require_test::test_EOF_SP_EOF() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << " %%EOF\n";

	stream_parser str_parser(std::move(stream));
	try {
		str_parser.require(require_type::keyword_EOF);
	} catch (const error_types::parse_error& e) {
		Assert::IsTrue(parse_error::keyword_EOF_not_found == e.code());

		// success
		return;
	}
	Assert::Fail();
}
void require_test::test_EOF_end_of_line() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream_parser str_parser(std::move(stream));
	try {
		str_parser.require(require_type::keyword_EOF);
	} catch (const error_types::parse_error& e) {
		Assert::IsTrue(parse_error::keyword_EOF_not_found == e.code());

		// success
		return;
	}
	Assert::Fail();
}

void require_test::test_startxref_startxref_EOL() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "startxref\r\n";
	std::streamoff expected = stream.tellp();

	stream_parser str_parser(std::move(stream));
	str_parser.require(require_type::keyword_startxref);
	Assert::IsTrue(expected == str_parser.tell());
}
void require_test::test_startxref_SP_startxref_comment_EOL() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << '\0' << "\x09\x0C\x20startxref" << '\0'
	       << "\x09\x0C\x20 % this is "
	          "comment.\r\n";
	std::streamoff expected = stream.tellp();

	stream_parser str_parser(std::move(stream));
	str_parser.require(require_type::keyword_startxref);
	Assert::IsTrue(expected == str_parser.tell());
}
void require_test::test_startxref_startxref_only() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "startxref";

	stream_parser str_parser(std::move(stream));
	try {
		str_parser.require(require_type::keyword_startxref);
	} catch (const parse_error& parse_e) {
		Assert::IsTrue(parse_error::EOL_not_found == parse_e.code());

		// success
		return;
	}
	Assert::Fail();
}
void require_test::test_startxref_startxref_not_EOL() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "startxref invalidtokens\n";

	stream_parser str_parser(std::move(stream));
	try {
		str_parser.require(require_type::keyword_startxref);
	} catch (const parse_error& parse_e) {
		Assert::IsTrue(parse_error::EOL_not_found == parse_e.code());

		// success
		return;
	}
	Assert::Fail();
}
void require_test::test_startxref_not_startxref() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "sstartxref\n";

	stream_parser str_parser(std::move(stream));
	try {
		str_parser.require(require_type::keyword_startxref);
	} catch (const parse_error& parse_e) {
		Assert::IsTrue(parse_error::keyword_startxref_not_found == parse_e.code());

		// success
		return;
	}
	Assert::Fail();
}
void require_test::test_startxref_end_of_line() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream_parser str_parser(std::move(stream));
	try {
		str_parser.require(require_type::keyword_startxref);
	} catch (const parse_error& parse_e) {
		Assert::IsTrue(parse_error::keyword_startxref_not_found == parse_e.code());

		// success
		return;
	}
	Assert::Fail();
}

void require_test::test_xref_xref_EOL() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "xref\r\n";
	std::streamoff expected = stream.tellp();

	stream_parser str_parser(std::move(stream));
	str_parser.require(require_type::keyword_xref);
	Assert::IsTrue(expected == str_parser.tell());
}
void require_test::test_xref_SP_xref_comment_EOL() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << '\0' << "\x09\x0C\x20xref" << '\0'
	       << "\x09\x0C\x20 % this is "
	          "comment.\r\n";
	std::streamoff expected = stream.tellp();

	stream_parser str_parser(std::move(stream));
	str_parser.require(require_type::keyword_xref);
	Assert::IsTrue(expected == str_parser.tell());
}
void require_test::test_xref_xref_only() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "xref";

	stream_parser str_parser(std::move(stream));
	try {
		str_parser.require(require_type::keyword_xref);
	} catch (const parse_error& parse_e) {
		Assert::IsTrue(parse_error::EOL_not_found == parse_e.code());

		// success
		return;
	}
	Assert::Fail();
}
void require_test::test_xref_xref_not_EOL() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "xref invalidtokens\n";

	stream_parser str_parser(std::move(stream));
	try {
		str_parser.require(require_type::keyword_xref);
	} catch (const parse_error& parse_e) {
		Assert::IsTrue(parse_error::EOL_not_found == parse_e.code());

		// success
		return;
	}
	Assert::Fail();
}
void require_test::test_xref_not_xref() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "xxref\n";

	stream_parser str_parser(std::move(stream));
	try {
		str_parser.require(require_type::keyword_xref);
	} catch (const parse_error& parse_e) {
		Assert::IsTrue(parse_error::keyword_xref_not_found == parse_e.code());

		// success
		return;
	}
	Assert::Fail();
}
void require_test::test_xref_end_of_line() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream_parser str_parser(std::move(stream));
	try {
		str_parser.require(require_type::keyword_xref);
	} catch (const parse_error& parse_e) {
		Assert::IsTrue(parse_error::keyword_xref_not_found == parse_e.code());

		// success
		return;
	}
	Assert::Fail();
}

void require_test::test_space_space_only() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << ' ';
	std::streamoff expected = stream.tellp();

	stream_parser str_parser(std::move(stream));
	str_parser.require(require_type::space);
	Assert::IsTrue(expected == str_parser.tell());
}
void require_test::test_space_not_space() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "\n";

	stream_parser str_parser(std::move(stream));
	try {
		str_parser.require(require_type::space);
	} catch (const parse_error& parse_e) {
		Assert::IsTrue(parse_error::space_not_found == parse_e.code());

		// success
		return;
	}
	Assert::Fail();
}
void require_test::test_space_end_of_line() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream_parser str_parser(std::move(stream));
	try {
		str_parser.require(require_type::space);
	} catch (const parse_error& parse_e) {
		Assert::IsTrue(parse_error::space_not_found == parse_e.code());

		// success
		return;
	}
	Assert::Fail();
}
#pragma endregion // region require_test

#pragma region parser_test
#include "parser_test.hpp"
using namespace pdfparser_test;
/**
 * helloworld.pdf の読み込みテスト
 */
void parser_test::test_normal_helloworld() {
	using namespace xref_types;

	parser main_parser{std::ifstream(
	    R"(..\..\test\UnitTest\PDFParserTest\data\helloworld.pdf)")};
	/*
	Assert::IsTrue(
	    xref_table{xref_free_entry{0, 65535, 0}, xref_inuse_entry{1, 0, 15},
	               xref_inuse_entry{2, 0, 66}, xref_inuse_entry{3, 0, 223},
	               xref_inuse_entry{4, 0, 125},
	               xref_inuse_entry{5, 0, 329}} == main_parser.get_xref_table());
	*/
}
#pragma endregion // parser_test
