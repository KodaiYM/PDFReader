#include "PDFParser.error_types.cpp"
#include "PDFParser.parser.cpp"
#include "PDFParser.xref_types.cpp"
#include "parser_test.hpp"
// TODO: コメント外して、定義を作る
// #include "require_test.hpp"
#include "seek_forward_head_of_line_test.hpp"
#include "take_xref_entry_test.hpp"
#include "take_xref_table_test.hpp"

#include <iomanip>
#include <sstream>

using namespace pdfparser_test;
using namespace pdfparser;

#pragma region seek_forward_head_of_line_test
void seek_forward_head_of_line_test::initialize() {
	ss = new std::stringstream(std::ios_base::in | std::ios_base::out |
	                           std::ios_base::binary);
	ss->exceptions(std::ios_base::failbit | std::ios_base::badbit);
}
void seek_forward_head_of_line_test::cleanup() {
	delete ss;
}
/**
 * | abc
 * def の時にエラーになるか
 */
void seek_forward_head_of_line_test::test_beginning_of_file() {
	*ss << "abc\ndef";

	try {
		seek_forward_head_of_line(*ss);
	} catch (std::ios_base::failure&) {
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
	*ss << "abc\n";

	auto expected = ss->tellp();

	*ss << "def\n";
	auto head_of_ghi = ss->tellp();

	*ss << "ghi\n";
	ss->seekg(head_of_ghi);

	seek_forward_head_of_line(*ss);
	Assert::IsTrue(expected == ss->tellg());
}
/**
 * abc
 * de | f の時に
 *
 * abc
 * | def になるか
 */
void seek_forward_head_of_line_test::test_middle_of_line() {
	*ss << "abc\n";

	auto expected = ss->tellp();

	*ss << "de";
	auto middle_of_def = ss->tellp();

	*ss << "f\n";
	ss->seekg(middle_of_def);

	seek_forward_head_of_line(*ss);
	Assert::IsTrue(expected == ss->tellg());
}
/**
 * CR | の時に
 * | CR になるか
 */
void seek_forward_head_of_line_test::test_CR() {
	auto expected = ss->tellp();

	*ss << "\r";
	ss->seekg(ss->tellp());

	seek_forward_head_of_line(*ss);
	Assert::IsTrue(expected == ss->tellg());
}
/**
 * LF | の時に
 * | LF になるか
 */
void seek_forward_head_of_line_test::test_LF() {
	auto expected = ss->tellp();

	*ss << "\n";
	ss->seekg(ss->tellp());

	seek_forward_head_of_line(*ss);
	Assert::IsTrue(expected == ss->tellg());
}
/**
 * CRLF | の時に
 * | CRLF になるか
 */
void seek_forward_head_of_line_test::test_CRLF() {
	auto expected = ss->tellp();

	*ss << "\r\n";
	ss->seekg(ss->tellp());

	seek_forward_head_of_line(*ss);
	Assert::IsTrue(expected == ss->tellg());
}
/**
 * c CR CR | の時に
 * c CR | CR になるか
 */
void seek_forward_head_of_line_test::test_CR_CRCR() {
	*ss << "c\r";
	auto expected = ss->tellp();

	*ss << "\r";
	ss->seekg(ss->tellp());

	seek_forward_head_of_line(*ss);
	Assert::IsTrue(expected == ss->tellg());
}
/**
 * c LF CR | の時に
 * c LF | CR になるか
 */
void seek_forward_head_of_line_test::test_CR_LFCR() {
	*ss << "c\n";
	auto expected = ss->tellp();

	*ss << "\r";
	ss->seekg(ss->tellp());

	seek_forward_head_of_line(*ss);
	Assert::IsTrue(expected == ss->tellg());
}
/**
 * c LF LF | の時に
 * c LF | LF になるか
 */
void seek_forward_head_of_line_test::test_LF_LFLF() {
	*ss << "c\n";
	auto expected = ss->tellp();

	*ss << "\n";
	ss->seekg(ss->tellp());

	seek_forward_head_of_line(*ss);
	Assert::IsTrue(expected == ss->tellg());
}
/**
 * c CR CRLF | の時に
 * c CR | CRLF になるか
 */
void seek_forward_head_of_line_test::test_CRLF_CRCRLF() {
	*ss << "c\r";
	auto expected = ss->tellp();

	*ss << "\r\n";
	ss->seekg(ss->tellp());

	seek_forward_head_of_line(*ss);
	Assert::IsTrue(expected == ss->tellg());
}
/**
 * c LF CRLF | の時に
 * c LF | CRLF になるか
 */
void seek_forward_head_of_line_test::test_CRLF_LFCRLF() {
	*ss << "c\n";
	auto expected = ss->tellp();

	*ss << "\r\n";
	ss->seekg(ss->tellp());

	seek_forward_head_of_line(*ss);
	Assert::IsTrue(expected == ss->tellg());
}
#pragma endregion // region seek_forward_head_of_line_test

#pragma region take_xref_table_test
// test largest xref table
void take_xref_table_test::test_maximum_xref_table() {
	std::stringstream ss(std::ios_base::in | std::ios_base::out |
	                     std::ios_base::binary);
	ss.exceptions(std::ios_base::failbit | std::ios_base::badbit);

	ss << "xref\n";

	try {
		ss << "1 " << std::numeric_limits<xref_types::object_t>::max() << "\n";
		take_xref_table(ss);
	} catch (syntax_error& syntax_e) {
		Assert::AreEqual(
		    static_cast<std::underlying_type_t<syntax_error::error_code>>(
		        syntax_error::xref_entry_first_10_digits_invalid),
		    static_cast<std::underlying_type_t<syntax_error::error_code>>(
		        syntax_e.code()));
		// success
		return;
	} finally {}
	Assert::Fail();
}
// test too large xref table (which occurs overflow_error)
void take_xref_table_test::test_overflow() {
	std::stringstream ss(std::ios_base::in | std::ios_base::out |
	                     std::ios_base::binary);
	ss.exceptions(std::ios_base::failbit | std::ios_base::badbit);

	ss << "xref\n";

	try {
		constexpr auto max = std::numeric_limits<xref_types::object_t>::max();
		ss << "1 " << (max / 10) << (max % 10 + 1) << "\n";
		take_xref_table(ss);
	} catch (overflow_or_underflow_error&) {
		// success
		return;
	} finally {}
	Assert::Fail();
}
#pragma endregion // region take_xref_table_test

#pragma region take_xref_entry_test
void take_xref_entry_test::test_SPCR() {
	using namespace xref_types;

	std::stringstream ss(std::ios_base::in | std::ios_base::out |
	                     std::ios_base::binary);
	ss.exceptions(std::ios_base::failbit | std::ios_base::badbit);

	constexpr object_t     object_number = 1;
	constexpr auto         next_free     = std::numeric_limits<object_t>::max();
	constexpr generation_t generation_number =
	    std::numeric_limits<generation_t>::max();
	ss << std::setfill('0');
	ss << std::setw(10) << next_free << " " << std::setw(5) << generation_number;
	ss << " f";
	ss << " \r";
	assert(ss.str().length() == 20);

	Assert::IsTrue(
	    take_xref_entry(ss, object_number) ==
	    xref_entry{xref_free_entry{object_number, generation_number, next_free}});
}
void take_xref_entry_test::test_SPLF() {
	using namespace xref_types;

	std::stringstream ss(std::ios_base::in | std::ios_base::out |
	                     std::ios_base::binary);
	ss.exceptions(std::ios_base::failbit | std::ios_base::badbit);

	constexpr object_t     object_number = 0;
	constexpr auto         next_free     = 0;
	constexpr generation_t generation_number =
	    std::numeric_limits<generation_t>::max();
	ss << std::setfill('0');
	ss << std::setw(10) << next_free << " " << std::setw(5) << generation_number;
	ss << " f";
	ss << " \n";
	assert(ss.str().length() == 20);

	Assert::IsTrue(
	    take_xref_entry(ss, object_number) ==
	    xref_entry{xref_free_entry{object_number, generation_number, next_free}});
}
void take_xref_entry_test::test_CRLF() {
	using namespace xref_types;

	std::stringstream ss(std::ios_base::in | std::ios_base::out |
	                     std::ios_base::binary);
	ss.exceptions(std::ios_base::failbit | std::ios_base::badbit);

	constexpr object_t     object_number = std::numeric_limits<object_t>::max();
	constexpr auto         byte_offset   = 9999999999;
	constexpr generation_t generation_number =
	    std::numeric_limits<generation_t>::max();
	ss << std::setfill('0');
	ss << std::setw(10) << byte_offset << " " << std::setw(5)
	   << generation_number;
	ss << " n";
	ss << "\r\n";
	assert(ss.str().length() == 20);

	Assert::IsTrue(take_xref_entry(ss, object_number) ==
	               xref_entry{xref_inuse_entry{object_number, generation_number,
	                                           byte_offset}});
}
#pragma endregion // take_xref_entry_test

// TODO: 作る
#pragma region require_test
#pragma endregion // region require_test

#pragma region parser_test
/**
 * helloworld.pdf の読み込みテスト
 */
void parser_test::test_normal_helloworld() {
	using namespace xref_types;

	parser parser{R"(..\..\test\UnitTest\PDFParserTest\data\helloworld.pdf)"};
	Assert::IsTrue(
	    xref_table{xref_free_entry{0, 65535, 0}, xref_inuse_entry{1, 0, 15},
	               xref_inuse_entry{2, 0, 66}, xref_inuse_entry{3, 0, 223},
	               xref_inuse_entry{4, 0, 125},
	               xref_inuse_entry{5, 0, 329}} == parser.get_xref_table());
}
#pragma endregion // parser_test
