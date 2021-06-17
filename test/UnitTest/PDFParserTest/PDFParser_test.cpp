#include "PDFParser.error_types.cpp"
#include "PDFParser.parser.cpp"
#include "PDFParser.xref_types.cpp"

#include <cassert>
#include <sstream>

using namespace Microsoft::VisualStudio::TestTools::UnitTesting;
using namespace pdfparser;

namespace pdfparser_test {
[TestClass] public ref class seek_forward_head_of_line_test {
public:
	[TestInitialize] void initialize();
	[TestCleanup] void    cleanup();

public:
	[TestMethod] void test_beginning_of_file();
	[TestMethod] void test_beginning_of_line();
	[TestMethod] void test_middle_of_line();
	[TestMethod] void test_CR();
	[TestMethod] void test_LF();
	[TestMethod] void test_CRLF();
	[TestMethod] void test_CR_CRCR();
	[TestMethod] void test_CR_LFCR();
	[TestMethod] void test_LF_LFLF();
	[TestMethod] void test_CRLF_CRCRLF();
	[TestMethod] void test_CRLF_LFCRLF();

private:
	std::stringstream* ss;
};
[TestClass] public ref class take_xref_table_test {
public:
	[TestMethod] void test_maximum_xref_table();
	[TestMethod] void test_overflow();
};
[TestClass] public ref class parser_test {
public:
	[TestMethod] void test_normal_helloworld();
};

/* definitions */
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
		throw gcnew AssertFailedException("didnt throw");
	} catch (std::ios_base::failure&) {
		// success
	}
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
// test largest xref table
void take_xref_table_test::test_maximum_xref_table() {
	std::stringstream ss(std::ios_base::in | std::ios_base::out |
	                     std::ios_base::binary);
	ss.exceptions(std::ios_base::failbit | std::ios_base::badbit);

	ss << "xref\n";

	try {
		ss << "1 " << std::numeric_limits<xref_types::object_t>::max() << "\n";
		take_xref_table(ss);

		throw gcnew AssertFailedException("didnt throw");
	} catch (overflow_or_underflow_error&) {
		throw gcnew AssertFailedException("overflow_or_underflow_error was thrown");
	} catch (syntax_error& syntax_e) {
		Assert::AreEqual(
		    static_cast<std::underlying_type_t<syntax_error::error_code>>(
		        syntax_error::xref_entry_first_10_digits_invalid),
		    static_cast<std::underlying_type_t<syntax_error::error_code>>(
		        syntax_e.code()));
	}
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

		throw gcnew AssertFailedException("didnt throw");
	} catch (syntax_error&) {
		throw gcnew AssertFailedException("syntax_error was thrown");
	} catch (overflow_or_underflow_error&) {
		// success
	}
}
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
} // namespace pdfparser_test
