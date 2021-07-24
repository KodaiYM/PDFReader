#include "PDFParser.error_types.cpp"
#include "PDFParser.parser.cpp"
#include "PDFParser.xref_types.cpp"

#include <iomanip>
#include <limits>
#include <sstream>

using namespace pdfparser;

#pragma region seek_forward_head_of_line_test
#include "seek_forward_head_of_line_test.hpp"
using namespace pdfparser_test;
void seek_forward_head_of_line_test::initialize() {
	m_ss = new std::stringstream(std::ios_base::in | std::ios_base::out |
	                             std::ios_base::binary);
	m_ss->exceptions(std::ios_base::failbit | std::ios_base::badbit);
}
void seek_forward_head_of_line_test::cleanup() {
	delete m_ss;
}
/**
 * | abc
 * def �̎��ɃG���[�ɂȂ邩
 */
void seek_forward_head_of_line_test::test_beginning_of_file() {
	*m_ss << "abc\ndef";

	try {
		seek_forward_head_of_line(*m_ss);
	} catch (std::ios_base::failure&) {
		// success
		return;
	}
	Assert::Fail();
}
/**
 * abc
 * def
 * | ghi �̎���
 *
 * abc
 * | def
 * ghi �ɂȂ邩
 */
void seek_forward_head_of_line_test::test_beginning_of_line() {
	*m_ss << "abc\n";

	auto expected = m_ss->tellp();

	*m_ss << "def\n";
	auto head_of_ghi = m_ss->tellp();

	*m_ss << "ghi\n";
	m_ss->seekg(head_of_ghi);

	seek_forward_head_of_line(*m_ss);
	Assert::IsTrue(expected == m_ss->tellg());
}
/**
 * abc
 * de | f �̎���
 *
 * abc
 * | def �ɂȂ邩
 */
void seek_forward_head_of_line_test::test_middle_of_line() {
	*m_ss << "abc\n";

	auto expected = m_ss->tellp();

	*m_ss << "de";
	auto middle_of_def = m_ss->tellp();

	*m_ss << "f\n";
	m_ss->seekg(middle_of_def);

	seek_forward_head_of_line(*m_ss);
	Assert::IsTrue(expected == m_ss->tellg());
}
/**
 * CR | �̎���
 * | CR �ɂȂ邩
 */
void seek_forward_head_of_line_test::test_CR() {
	auto expected = m_ss->tellp();

	*m_ss << "\r";
	m_ss->seekg(m_ss->tellp());

	seek_forward_head_of_line(*m_ss);
	Assert::IsTrue(expected == m_ss->tellg());
}
/**
 * LF | �̎���
 * | LF �ɂȂ邩
 */
void seek_forward_head_of_line_test::test_LF() {
	auto expected = m_ss->tellp();

	*m_ss << "\n";
	m_ss->seekg(m_ss->tellp());

	seek_forward_head_of_line(*m_ss);
	Assert::IsTrue(expected == m_ss->tellg());
}
/**
 * CRLF | �̎���
 * | CRLF �ɂȂ邩
 */
void seek_forward_head_of_line_test::test_CRLF() {
	auto expected = m_ss->tellp();

	*m_ss << "\r\n";
	m_ss->seekg(m_ss->tellp());

	seek_forward_head_of_line(*m_ss);
	Assert::IsTrue(expected == m_ss->tellg());
}
/**
 * c CR CR | �̎���
 * c CR | CR �ɂȂ邩
 */
void seek_forward_head_of_line_test::test_CR_CRCR() {
	*m_ss << "c\r";
	auto expected = m_ss->tellp();

	*m_ss << "\r";
	m_ss->seekg(m_ss->tellp());

	seek_forward_head_of_line(*m_ss);
	Assert::IsTrue(expected == m_ss->tellg());
}
/**
 * c LF CR | �̎���
 * c LF | CR �ɂȂ邩
 */
void seek_forward_head_of_line_test::test_CR_LFCR() {
	*m_ss << "c\n";
	auto expected = m_ss->tellp();

	*m_ss << "\r";
	m_ss->seekg(m_ss->tellp());

	seek_forward_head_of_line(*m_ss);
	Assert::IsTrue(expected == m_ss->tellg());
}
/**
 * c LF LF | �̎���
 * c LF | LF �ɂȂ邩
 */
void seek_forward_head_of_line_test::test_LF_LFLF() {
	*m_ss << "c\n";
	auto expected = m_ss->tellp();

	*m_ss << "\n";
	m_ss->seekg(m_ss->tellp());

	seek_forward_head_of_line(*m_ss);
	Assert::IsTrue(expected == m_ss->tellg());
}
/**
 * c CR CRLF | �̎���
 * c CR | CRLF �ɂȂ邩
 */
void seek_forward_head_of_line_test::test_CRLF_CRCRLF() {
	*m_ss << "c\r";
	auto expected = m_ss->tellp();

	*m_ss << "\r\n";
	m_ss->seekg(m_ss->tellp());

	seek_forward_head_of_line(*m_ss);
	Assert::IsTrue(expected == m_ss->tellg());
}
/**
 * c LF CRLF | �̎���
 * c LF | CRLF �ɂȂ邩
 */
void seek_forward_head_of_line_test::test_CRLF_LFCRLF() {
	*m_ss << "c\n";
	auto expected = m_ss->tellp();

	*m_ss << "\r\n";
	m_ss->seekg(m_ss->tellp());

	seek_forward_head_of_line(*m_ss);
	Assert::IsTrue(expected == m_ss->tellg());
}
#pragma endregion // region seek_forward_head_of_line_test

#pragma region take_xref_table_test
#include "take_xref_table_test.hpp"
using namespace pdfparser_test;
void take_xref_table_test::initialize() {
	m_ss = new std::stringstream(std::ios_base::in | std::ios_base::out |
	                             std::ios_base::binary);
	m_ss->exceptions(std::ios_base::failbit | std::ios_base::badbit);
}
void take_xref_table_test::cleanup() {
	delete m_ss;
}
// test largest xref table
void take_xref_table_test::test_maximum_xref_table() {
	*m_ss << "xref\n";

	try {
		*m_ss << "1 " << std::numeric_limits<xref_types::object_t>::max() << "\n";
		take_xref_table(*m_ss);
	} catch (const syntax_error& syntax_e) {
		Assert::IsTrue(syntax_error::xref_entry_first_10_digits_invalid ==
		               syntax_e.code());

		// success
		return;
	}
	Assert::Fail();
}
// test too large xref table (which occurs std::overflow_error)
void take_xref_table_test::test_overflow() {
	*m_ss << "xref\n";

	try {
		constexpr auto max = std::numeric_limits<xref_types::object_t>::max();
		*m_ss << "1 " << (max / 10) << (max % 10 + 1) << "\n";
		take_xref_table(*m_ss);
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
void take_xref_entry_test::initialize() {
	m_ss = new std::stringstream(std::ios_base::in | std::ios_base::out |
	                             std::ios_base::binary);
	m_ss->exceptions(std::ios_base::failbit | std::ios_base::badbit);
}
void take_xref_entry_test::cleanup() {
	delete m_ss;
}
void take_xref_entry_test::test_SPCR() {
	using namespace xref_types;

	constexpr object_t     object_number = 1;
	constexpr auto         next_free     = std::numeric_limits<object_t>::max();
	constexpr generation_t generation_number =
	    std::numeric_limits<generation_t>::max();
	*m_ss << std::setfill('0');
	*m_ss << std::setw(10) << next_free << " " << std::setw(5)
	      << generation_number;
	*m_ss << " f";
	*m_ss << " \r";
	assert(m_ss->str().length() == 20);

	Assert::IsTrue(
	    take_xref_entry(*m_ss, object_number) ==
	    xref_entry{xref_free_entry{object_number, generation_number, next_free}});
}
void take_xref_entry_test::test_SPLF() {
	using namespace xref_types;

	constexpr object_t     object_number = 0;
	constexpr auto         next_free     = 0;
	constexpr generation_t generation_number =
	    std::numeric_limits<generation_t>::max();
	*m_ss << std::setfill('0');
	*m_ss << std::setw(10) << next_free << " " << std::setw(5)
	      << generation_number;
	*m_ss << " f";
	*m_ss << " \n";
	assert(m_ss->str().length() == 20);

	Assert::IsTrue(
	    take_xref_entry(*m_ss, object_number) ==
	    xref_entry{xref_free_entry{object_number, generation_number, next_free}});
}
void take_xref_entry_test::test_CRLF() {
	using namespace xref_types;

	constexpr object_t     object_number = std::numeric_limits<object_t>::max();
	constexpr auto         byte_offset   = 9999999999;
	constexpr generation_t generation_number =
	    std::numeric_limits<generation_t>::max();
	*m_ss << std::setfill('0');
	*m_ss << std::setw(10) << byte_offset << " " << std::setw(5)
	      << generation_number;
	*m_ss << " n";
	*m_ss << "\r\n";
	assert(m_ss->str().length() == 20);

	Assert::IsTrue(take_xref_entry(*m_ss, object_number) ==
	               xref_entry{xref_inuse_entry{object_number, generation_number,
	                                           byte_offset}});
}
#pragma endregion // take_xref_entry_test

#pragma region ignore_if_present_test
#include "ignore_if_present_test.hpp"
using namespace pdfparser_test;
void ignore_if_present_test::initialize() {
	m_ss = new std::stringstream(std::ios_base::in | std::ios_base::out |
	                             std::ios_base::binary);
	m_ss->exceptions(std::ios_base::failbit | std::ios_base::badbit);
}
void ignore_if_present_test::cleanup() {
	delete m_ss;
}

void ignore_if_present_test::test_null_only() {
	*m_ss << '\0' << " \t\n\f\r ";
	ignore_if_present(*m_ss, ignore_flag::null);
	Assert::IsTrue(1 == m_ss->tellg());
}
void ignore_if_present_test::test_line_feed_only() {
	*m_ss << "\n\r";
	ignore_if_present(*m_ss, ignore_flag::line_feed);
	Assert::IsTrue(1 == m_ss->tellg());
}
void ignore_if_present_test::test_form_feed_only() {
	*m_ss << '\f' << " \t\n\f\r ";
	ignore_if_present(*m_ss, ignore_flag::form_feed);
	Assert::IsTrue(1 == m_ss->tellg());
}
void ignore_if_present_test::test_carriage_return_only() {
	*m_ss << "\r\n";
	ignore_if_present(*m_ss, ignore_flag::carriage_return);
	Assert::IsTrue(1 == m_ss->tellg());
}
void ignore_if_present_test::test_space_only() {
	*m_ss << ' ' << "\t\n\f\r ";
	ignore_if_present(*m_ss, ignore_flag::space);
	Assert::IsTrue(1 == m_ss->tellg());
}
void ignore_if_present_test::test_comment_only() {
	*m_ss << "% this is comment section\n\r";
	ignore_if_present(*m_ss, ignore_flag::comment);
	Assert::IsTrue('\n' == m_ss->get());
}
void ignore_if_present_test::test_EOL_only() {
	*m_ss << "\r\n ";
	ignore_if_present(*m_ss, ignore_flag::EOL);
	Assert::IsTrue(2 == m_ss->tellg());
}
void ignore_if_present_test::test_any_whitespace_characters() {
	*m_ss << '\0' << "\t\n\f\r ";
	ignore_if_present(*m_ss, ignore_flag::any_whitespace_characters);
	Assert::IsTrue(m_ss->eof() ||
	               std::decay_t<decltype(*m_ss)>::traits_type::eof() ==
	                   m_ss->peek());
}
void ignore_if_present_test::test_all_whitespaces_including_comment() {
	*m_ss << '\0' << "\t\n\f\r %comment section\r\n"
	      << "\t\n\r\n " << '\0' << "\r\n %comment 2 section!\r"
	      << "% last comment !\n ";
	ignore_if_present(*m_ss, ignore_flag::any_whitespace_characters |
	                             ignore_flag::comment);
	Assert::IsTrue(m_ss->eof() ||
	               std::decay_t<decltype(*m_ss)>::traits_type::eof() ==
	                   m_ss->peek());
}
void ignore_if_present_test::test_nothing_to_ignore() {
	*m_ss << "don't ignore\r\n";
	ignore_if_present(*m_ss, ignore_flag::any_whitespace_characters |
	                             ignore_flag::comment);
	Assert::IsTrue(0 == m_ss->tellg());
}
void ignore_if_present_test::test_no_flags() {
	*m_ss << '\0' << "\t\n\f\r ";
	ignore_if_present(*m_ss, ignore_flag::null & ~ignore_flag::null);
	Assert::IsTrue(0 == m_ss->tellg());
}
#pragma endregion // region ignore_if_present_test

#pragma region take_signed_integer_test
#include "take_signed_integer_test.hpp"
using namespace pdfparser_test;
void take_signed_integer_test::initialize() {
	m_ss = new std::stringstream(std::ios_base::in | std::ios_base::out |
	                             std::ios_base::binary);
	m_ss->exceptions(std::ios_base::failbit | std::ios_base::badbit);
}
void take_signed_integer_test::cleanup() {
	delete m_ss;
}

void take_signed_integer_test::test_unsigned() {
	*m_ss << "100";
	auto integer = take_signed_integer<long long>(*m_ss);
	Assert::AreEqual((long long)100, integer);
	Assert::IsTrue(m_ss->eof() ||
	               std::decay_t<decltype(*m_ss)>::traits_type::eof() ==
	                   m_ss->peek());
}
void take_signed_integer_test::test_plus() {
	*m_ss << "+100";
	auto integer = take_signed_integer<long long>(*m_ss);
	Assert::AreEqual((long long)100, integer);
	Assert::IsTrue(m_ss->eof() ||
	               std::decay_t<decltype(*m_ss)>::traits_type::eof() ==
	                   m_ss->peek());
}
void take_signed_integer_test::test_minus() {
	*m_ss << "-100";
	auto integer = take_signed_integer<long long>(*m_ss);
	Assert::AreEqual((long long)-100, integer);
	Assert::IsTrue(m_ss->eof() ||
	               std::decay_t<decltype(*m_ss)>::traits_type::eof() ==
	                   m_ss->peek());
}
void take_signed_integer_test::test_max() {
	constexpr auto max = std::numeric_limits<long long>::max();
	*m_ss << max;
	auto integer = take_signed_integer<long long>(*m_ss);
	Assert::AreEqual(max, integer);
	Assert::IsTrue(m_ss->eof() ||
	               std::decay_t<decltype(*m_ss)>::traits_type::eof() ==
	                   m_ss->peek());
}
void take_signed_integer_test::test_min() {
	constexpr auto min = std::numeric_limits<long long>::min();
	*m_ss << min;
	auto integer = take_signed_integer<long long>(*m_ss);
	Assert::AreEqual(min, integer);
	Assert::IsTrue(m_ss->eof() ||
	               std::decay_t<decltype(*m_ss)>::traits_type::eof() ==
	                   m_ss->peek());
}

void take_signed_integer_test::test_not_number() {
	*m_ss << "ABC";
	try {
		take_signed_integer<long long>(*m_ss);
	} catch (const syntax_error& syntax_e) {
		Assert::IsTrue(syntax_error::signed_integer_not_found == syntax_e.code());

		// success
		return;
	}
	Assert::Fail();
}
void take_signed_integer_test::test_EOF() {
	try {
		take_signed_integer<long long>(*m_ss);
	} catch (const syntax_error& syntax_e) {
		Assert::IsTrue(syntax_error::signed_integer_not_found == syntax_e.code());

		// success
		return;
	}
	Assert::Fail();
}
void take_signed_integer_test::test_plus_not_number() {
	*m_ss << "+deadbeef";
	try {
		take_signed_integer<long long>(*m_ss);
	} catch (const syntax_error& syntax_e) {
		Assert::IsTrue(syntax_error::signed_integer_not_found == syntax_e.code());

		// success
		return;
	}
	Assert::Fail();
}
void take_signed_integer_test::test_minus_not_number() {
	*m_ss << "-deadbeef";
	try {
		take_signed_integer<long long>(*m_ss);
	} catch (const syntax_error& syntax_e) {
		Assert::IsTrue(syntax_error::signed_integer_not_found == syntax_e.code());

		// success
		return;
	}
	Assert::Fail();
}
void take_signed_integer_test::test_sign_EOF() {
	*m_ss << "+";
	try {
		take_signed_integer<long long>(*m_ss);
	} catch (const syntax_error& syntax_e) {
		Assert::IsTrue(syntax_error::signed_integer_not_found == syntax_e.code());

		// success
		return;
	}
	Assert::Fail();
}
void take_signed_integer_test::test_max_plus1() {
	constexpr auto max = std::numeric_limits<long long>::max();
	*m_ss << max / 10 << ((max % 10) + 1);
	try {
		take_signed_integer<long long>(*m_ss);
	} catch (std::overflow_error&) {
		// success
		return;
	}
	Assert::Fail();
}
void take_signed_integer_test::test_min_minus1() {
	constexpr auto min = std::numeric_limits<long long>::min();
	*m_ss << min / 10 << (-(min % 10) + 1);
	try {
		take_signed_integer<long long>(*m_ss);
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
void take_unsigned_integer_test::initialize() {
	m_ss = new std::stringstream(std::ios_base::in | std::ios_base::out |
	                             std::ios_base::binary);
	m_ss->exceptions(std::ios_base::failbit | std::ios_base::badbit);
}
void take_unsigned_integer_test::cleanup() {
	delete m_ss;
}

void take_unsigned_integer_test::test_unsigned() {
	*m_ss << "17";
	auto integer = take_unsigned_integer<long long>(*m_ss);
	Assert::AreEqual((long long)17, integer);
	Assert::IsTrue(m_ss->eof() ||
	               std::decay_t<decltype(*m_ss)>::traits_type::eof() ==
	                   m_ss->peek());
}
void take_unsigned_integer_test::test_max() {
	constexpr auto max = std::numeric_limits<unsigned long long>::max();
	*m_ss << max;
	auto integer = take_unsigned_integer<unsigned long long>(*m_ss);
	Assert::AreEqual(max, integer);
	Assert::IsTrue(m_ss->eof() ||
	               std::decay_t<decltype(*m_ss)>::traits_type::eof() ==
	                   m_ss->peek());
}
void take_unsigned_integer_test::test_min() {
	*m_ss << 0;
	auto integer = take_unsigned_integer<unsigned long long>(*m_ss);
	Assert::AreEqual((unsigned long long)0, integer);
	Assert::IsTrue(m_ss->eof() ||
	               std::decay_t<decltype(*m_ss)>::traits_type::eof() ==
	                   m_ss->peek());
}

void take_unsigned_integer_test::test_not_number() {
	*m_ss << "deadbeef";
	try {
		take_unsigned_integer<long long>(*m_ss);
	} catch (const syntax_error& syntax_e) {
		Assert::IsTrue(syntax_error::unsigned_integer_not_found == syntax_e.code());

		// success
		return;
	}
	Assert::Fail();
}
void take_unsigned_integer_test::test_EOF() {
	try {
		take_unsigned_integer<long long>(*m_ss);
	} catch (const syntax_error& syntax_e) {
		Assert::IsTrue(syntax_error::unsigned_integer_not_found == syntax_e.code());

		// success
		return;
	}
	Assert::Fail();
}
void take_unsigned_integer_test::test_signed() {
	*m_ss << "+17";
	try {
		take_unsigned_integer<long long>(*m_ss);
	} catch (const syntax_error& syntax_e) {
		Assert::IsTrue(syntax_error::unsigned_integer_not_found == syntax_e.code());

		// success
		return;
	}
	Assert::Fail();
}
void take_unsigned_integer_test::test_max_plus1() {
	constexpr auto max = std::numeric_limits<unsigned long long>::max();
	*m_ss << (max / 10) << (max % 10 + 1);

	try {
		take_unsigned_integer<long long>(*m_ss);
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
void require_test::initialize() {
	m_ss = new std::stringstream(std::ios_base::in | std::ios_base::out |
	                             std::ios_base::binary);
	m_ss->exceptions(std::ios_base::failbit | std::ios_base::badbit);
}
void require_test::cleanup() {
	delete m_ss;
}
void require_test::test_EOF_EOF_EOL() {
	*m_ss << "%%EOF\r\n";
	require(*m_ss, require_type::keyword_EOF);
	Assert::IsTrue(m_ss->eof() ||
	               std::decay_t<decltype(*m_ss)>::traits_type::eof() ==
	                   m_ss->peek());
}
void require_test::test_EOF_EOF_only() {
	*m_ss << "%%EOF";
	require(*m_ss, require_type::keyword_EOF);
	Assert::IsTrue(m_ss->eof() ||
	               std::decay_t<decltype(*m_ss)>::traits_type::eof() ==
	                   m_ss->peek());
}
void require_test::test_EOF_EOF_not_EOL() {
	*m_ss << "%%EOF ";
	try {
		require(*m_ss, require_type::keyword_EOF);
	} catch (const error_types::syntax_error& e) {
		Assert::IsTrue(syntax_error::EOL_not_found == e.code());

		// success
		return;
	}
	Assert::Fail();
}
void require_test::test_EOF_not_EOF() {
	*m_ss << "%EOF\n";
	try {
		require(*m_ss, require_type::keyword_EOF);
	} catch (const error_types::syntax_error& e) {
		Assert::IsTrue(syntax_error::EOF_not_found == e.code());

		// success
		return;
	}
	Assert::Fail();
}
void require_test::test_EOF_SP_EOF() {
	*m_ss << " %%EOF\n";
	try {
		require(*m_ss, require_type::keyword_EOF);
	} catch (const error_types::syntax_error& e) {
		Assert::IsTrue(syntax_error::EOF_not_found == e.code());

		// success
		return;
	}
	Assert::Fail();
}
void require_test::test_EOF_end_of_line() {
	try {
		require(*m_ss, require_type::keyword_EOF);
	} catch (const error_types::syntax_error& e) {
		Assert::IsTrue(syntax_error::EOF_not_found == e.code());

		// success
		return;
	}
	Assert::Fail();
}

void require_test::test_startxref_startxref_EOL() {
	*m_ss << "startxref\r\n";
	require(*m_ss, require_type::keyword_startxref);
	Assert::IsTrue(m_ss->eof() ||
	               std::decay_t<decltype(*m_ss)>::traits_type::eof() ==
	                   m_ss->peek());
}
void require_test::test_startxref_SP_startxref_comment_EOL() {
	*m_ss << '\0' << "\x09\x0C\x20startxref" << '\0'
	      << "\x09\x0C\x20 % this is "
	         "comment.\r\n";

	require(*m_ss, require_type::keyword_startxref);
	Assert::IsTrue(m_ss->eof() ||
	               std::decay_t<decltype(*m_ss)>::traits_type::eof() ==
	                   m_ss->peek());
}
void require_test::test_startxref_startxref_only() {
	*m_ss << "startxref";
	try {
		require(*m_ss, require_type::keyword_startxref);
	} catch (const syntax_error& syntax_e) {
		Assert::IsTrue(syntax_error::EOL_not_found == syntax_e.code());

		// success
		return;
	}
	Assert::Fail();
}
void require_test::test_startxref_startxref_not_EOL() {
	*m_ss << "startxref invalidtokens\n";
	try {
		require(*m_ss, require_type::keyword_startxref);
	} catch (const syntax_error& syntax_e) {
		Assert::IsTrue(syntax_error::EOL_not_found == syntax_e.code());

		// success
		return;
	}
	Assert::Fail();
}
void require_test::test_startxref_not_startxref() {
	*m_ss << "sstartxref\n";
	try {
		require(*m_ss, require_type::keyword_startxref);
	} catch (const syntax_error& syntax_e) {
		Assert::IsTrue(syntax_error::keyword_startxref_not_found ==
		               syntax_e.code());

		// success
		return;
	}
	Assert::Fail();
}
void require_test::test_startxref_end_of_line() {
	try {
		require(*m_ss, require_type::keyword_startxref);
	} catch (const syntax_error& syntax_e) {
		Assert::IsTrue(syntax_error::keyword_startxref_not_found ==
		               syntax_e.code());

		// success
		return;
	}
	Assert::Fail();
}

void require_test::test_xref_xref_EOL() {
	*m_ss << "xref\r\n";
	require(*m_ss, require_type::keyword_xref);
	Assert::IsTrue(m_ss->eof() ||
	               std::decay_t<decltype(*m_ss)>::traits_type::eof() ==
	                   m_ss->peek());
}
void require_test::test_xref_SP_xref_comment_EOL() {
	*m_ss << '\0' << "\x09\x0C\x20xref" << '\0'
	      << "\x09\x0C\x20 % this is "
	         "comment.\r\n";
	require(*m_ss, require_type::keyword_xref);
	Assert::IsTrue(m_ss->eof() ||
	               std::decay_t<decltype(*m_ss)>::traits_type::eof() ==
	                   m_ss->peek());
}
void require_test::test_xref_xref_only() {
	*m_ss << "xref";
	try {
		require(*m_ss, require_type::keyword_xref);
	} catch (const syntax_error& syntax_e) {
		Assert::IsTrue(syntax_error::EOL_not_found == syntax_e.code());

		// success
		return;
	}
	Assert::Fail();
}
void require_test::test_xref_xref_not_EOL() {
	*m_ss << "xref invalidtokens\n";
	try {
		require(*m_ss, require_type::keyword_xref);
	} catch (const syntax_error& syntax_e) {
		Assert::IsTrue(syntax_error::EOL_not_found == syntax_e.code());

		// success
		return;
	}
	Assert::Fail();
}
void require_test::test_xref_not_xref() {
	*m_ss << "xxref\n";
	try {
		require(*m_ss, require_type::keyword_xref);
	} catch (const syntax_error& syntax_e) {
		Assert::IsTrue(syntax_error::keyword_xref_not_found == syntax_e.code());

		// success
		return;
	}
	Assert::Fail();
}
void require_test::test_xref_end_of_line() {
	try {
		require(*m_ss, require_type::keyword_xref);
	} catch (const syntax_error& syntax_e) {
		Assert::IsTrue(syntax_error::keyword_xref_not_found == syntax_e.code());

		// success
		return;
	}
	Assert::Fail();
}

void require_test::test_space_space_only() {
	*m_ss << ' ';
	require(*m_ss, require_type::space);
	Assert::IsTrue(m_ss->eof() ||
	               std::decay_t<decltype(*m_ss)>::traits_type::eof() ==
	                   m_ss->peek());
}
void require_test::test_space_not_space() {
	*m_ss << "\n";
	try {
		require(*m_ss, require_type::space);
	} catch (const syntax_error& syntax_e) {
		Assert::IsTrue(syntax_error::space_not_found == syntax_e.code());

		// success
		return;
	}
	Assert::Fail();
}
void require_test::test_space_end_of_line() {
	try {
		require(*m_ss, require_type::space);
	} catch (const syntax_error& syntax_e) {
		Assert::IsTrue(syntax_error::space_not_found == syntax_e.code());

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
 * helloworld.pdf �̓ǂݍ��݃e�X�g
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