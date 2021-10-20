#include "pdfparser.istream_extended.hpp"
#include "pdfparser.istream_extended_errors.hpp"
#include "seek_forward_head_of_line_test.hpp"

#include <sstream>

using namespace pdfparser;
using namespace istream_extended_test;

/**
 * | abc
 * def ‚Ì‚ÉƒGƒ‰[‚É‚È‚é‚©
 */
void seek_forward_head_of_line_test::test_beginning_of_file() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);
	stream << "abc\ndef";

	istream_extended str_extended(stream.rdbuf());
	try {
		str_extended.seek_forward_head_of_line();
	} catch (const failed_to_seek_forward_head_of_line& e) {
		Assert::IsTrue(0 == e.tell_position());

		// success
		return;
	}
	Assert::Fail();
}
/**
 * abc
 * def
 * | ghi ‚Ì‚É
 *
 * abc
 * | def
 * ghi ‚É‚È‚é‚©
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

	istream_extended str_extended(stream.rdbuf());
	str_extended.seek_forward_head_of_line();
	Assert::IsTrue(expected == str_extended.tell());
}
/**
 * abc
 * de | f ‚Ì‚É
 *
 * abc
 * | def ‚É‚È‚é‚©
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

	istream_extended str_extended(stream.rdbuf());
	str_extended.seek_forward_head_of_line();
	Assert::IsTrue(expected == str_extended.tell());
}
/**
 * CR | ‚Ì‚É
 * | CR ‚É‚È‚é‚©
 */
void seek_forward_head_of_line_test::test_CR_only() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	std::streamoff expected = stream.tellp();
	stream << "\r";
	stream.seekg(stream.tellp());

	istream_extended str_extended(stream.rdbuf());
	str_extended.seek_forward_head_of_line();
	Assert::IsTrue(expected == str_extended.tell());
}
/**
 * LF | ‚Ì‚É
 * | LF ‚É‚È‚é‚©
 */
void seek_forward_head_of_line_test::test_LF_only() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	std::streamoff expected = stream.tellp();
	stream << "\n";
	stream.seekg(stream.tellp());

	istream_extended str_extended(stream.rdbuf());
	str_extended.seek_forward_head_of_line();
	Assert::IsTrue(expected == str_extended.tell());
}
/**
 * CRLF | ‚Ì‚É
 * | CRLF ‚É‚È‚é‚©
 */
void seek_forward_head_of_line_test::test_CRLF_only() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	std::streamoff expected = stream.tellp();
	stream << "\r\n";
	stream.seekg(stream.tellp());

	istream_extended str_extended(stream.rdbuf());
	str_extended.seek_forward_head_of_line();
	Assert::IsTrue(expected == str_extended.tell());
}
/**
 * CR | ‚Ì‚É
 * | CR ‚É‚È‚é‚©
 */
void seek_forward_head_of_line_test::test_CR() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	std::streamoff expected = stream.tellp();
	stream << "xxx\r";
	stream.seekg(stream.tellp());

	istream_extended str_extended(stream.rdbuf());
	str_extended.seek_forward_head_of_line();
	Assert::IsTrue(expected == str_extended.tell());
}
/**
 * xxx LF | ‚Ì‚É
 * | xxx LF ‚É‚È‚é‚©
 */
void seek_forward_head_of_line_test::test_LF() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	std::streamoff expected = stream.tellp();
	stream << "xxx\n";
	stream.seekg(stream.tellp());

	istream_extended str_extended(stream.rdbuf());
	str_extended.seek_forward_head_of_line();
	Assert::IsTrue(expected == str_extended.tell());
}
/**
 * xxx CRLF | ‚Ì‚É
 * | xxx CRLF ‚É‚È‚é‚©
 */
void seek_forward_head_of_line_test::test_CRLF() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	std::streamoff expected = stream.tellp();
	stream << "xxx\r\n";
	stream.seekg(stream.tellp());

	istream_extended str_extended(stream.rdbuf());
	str_extended.seek_forward_head_of_line();
	Assert::IsTrue(expected == str_extended.tell());
}
/**
 * c CR CR | ‚Ì‚É
 * c CR | CR ‚É‚È‚é‚©
 */
void seek_forward_head_of_line_test::test_CR_CRCR() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "c\r";
	std::streamoff expected = stream.tellp();

	stream << "\r";
	stream.seekg(stream.tellp());

	istream_extended str_extended(stream.rdbuf());
	str_extended.seek_forward_head_of_line();
	Assert::IsTrue(expected == str_extended.tell());
}
/**
 * c LF CR | ‚Ì‚É
 * c LF | CR ‚É‚È‚é‚©
 */
void seek_forward_head_of_line_test::test_CR_LFCR() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "c\n";
	std::streamoff expected = stream.tellp();

	stream << "\r";
	stream.seekg(stream.tellp());

	istream_extended str_extended(stream.rdbuf());
	str_extended.seek_forward_head_of_line();
	Assert::IsTrue(expected == str_extended.tell());
}
/**
 * c LF LF | ‚Ì‚É
 * c LF | LF ‚É‚È‚é‚©
 */
void seek_forward_head_of_line_test::test_LF_LFLF() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "c\n";
	std::streamoff expected = stream.tellp();

	stream << "\n";
	stream.seekg(stream.tellp());

	istream_extended str_extended(stream.rdbuf());
	str_extended.seek_forward_head_of_line();
	Assert::IsTrue(expected == str_extended.tell());
}
/**
 * c CR CRLF | ‚Ì‚É
 * c CR | CRLF ‚É‚È‚é‚©
 */
void seek_forward_head_of_line_test::test_CRLF_CRCRLF() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "c\r";
	std::streamoff expected = stream.tellp();

	stream << "\r\n";
	stream.seekg(stream.tellp());

	istream_extended str_extended(stream.rdbuf());
	str_extended.seek_forward_head_of_line();
	Assert::IsTrue(expected == str_extended.tell());
}
/**
 * c LF CRLF | ‚Ì‚É
 * c LF | CRLF ‚É‚È‚é‚©
 */
void seek_forward_head_of_line_test::test_CRLF_LFCRLF() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "c\n";
	std::streamoff expected = stream.tellp();

	stream << "\r\n";
	stream.seekg(stream.tellp());

	istream_extended str_extended(stream.rdbuf());
	str_extended.seek_forward_head_of_line();
	Assert::IsTrue(expected == str_extended.tell());
}
