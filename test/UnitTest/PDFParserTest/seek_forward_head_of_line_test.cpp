#include "pdfparser.istream_extended.hpp"
#include "seek_forward_head_of_line_test.hpp"

#include <sstream>

using namespace pdfparser;
using namespace pdfparser_test;

/**
 * | abc
 * def の時にエラーになるか
 */
void seek_forward_head_of_line_test::test_beginning_of_file() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);
	stream << "abc\ndef";

	istream_extended str_extended(std::move(stream));
	try {
		str_extended.seek_forward_head_of_line();
	} catch (istream_extended_error& parse_e) {
		Assert::IsTrue(
		    istream_extended_error::failed_to_seek_forward_head_of_line ==
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

	istream_extended str_extended(std::move(stream));
	str_extended.seek_forward_head_of_line();
	Assert::IsTrue(expected == str_extended.tell());
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

	istream_extended str_extended(std::move(stream));
	str_extended.seek_forward_head_of_line();
	Assert::IsTrue(expected == str_extended.tell());
}
/**
 * CR | の時に
 * | CR になるか
 */
void seek_forward_head_of_line_test::test_CR_only() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	std::streamoff expected = stream.tellp();
	stream << "\r";
	stream.seekg(stream.tellp());

	istream_extended str_extended(std::move(stream));
	str_extended.seek_forward_head_of_line();
	Assert::IsTrue(expected == str_extended.tell());
}
/**
 * LF | の時に
 * | LF になるか
 */
void seek_forward_head_of_line_test::test_LF_only() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	std::streamoff expected = stream.tellp();
	stream << "\n";
	stream.seekg(stream.tellp());

	istream_extended str_extended(std::move(stream));
	str_extended.seek_forward_head_of_line();
	Assert::IsTrue(expected == str_extended.tell());
}
/**
 * CRLF | の時に
 * | CRLF になるか
 */
void seek_forward_head_of_line_test::test_CRLF_only() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	std::streamoff expected = stream.tellp();
	stream << "\r\n";
	stream.seekg(stream.tellp());

	istream_extended str_extended(std::move(stream));
	str_extended.seek_forward_head_of_line();
	Assert::IsTrue(expected == str_extended.tell());
}
/**
 * CR | の時に
 * | CR になるか
 */
void seek_forward_head_of_line_test::test_CR() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	std::streamoff expected = stream.tellp();
	stream << "xxx\r";
	stream.seekg(stream.tellp());

	istream_extended str_extended(std::move(stream));
	str_extended.seek_forward_head_of_line();
	Assert::IsTrue(expected == str_extended.tell());
}
/**
 * xxx LF | の時に
 * | xxx LF になるか
 */
void seek_forward_head_of_line_test::test_LF() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	std::streamoff expected = stream.tellp();
	stream << "xxx\n";
	stream.seekg(stream.tellp());

	istream_extended str_extended(std::move(stream));
	str_extended.seek_forward_head_of_line();
	Assert::IsTrue(expected == str_extended.tell());
}
/**
 * xxx CRLF | の時に
 * | xxx CRLF になるか
 */
void seek_forward_head_of_line_test::test_CRLF() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	std::streamoff expected = stream.tellp();
	stream << "xxx\r\n";
	stream.seekg(stream.tellp());

	istream_extended str_extended(std::move(stream));
	str_extended.seek_forward_head_of_line();
	Assert::IsTrue(expected == str_extended.tell());
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

	istream_extended str_extended(std::move(stream));
	str_extended.seek_forward_head_of_line();
	Assert::IsTrue(expected == str_extended.tell());
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

	istream_extended str_extended(std::move(stream));
	str_extended.seek_forward_head_of_line();
	Assert::IsTrue(expected == str_extended.tell());
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

	istream_extended str_extended(std::move(stream));
	str_extended.seek_forward_head_of_line();
	Assert::IsTrue(expected == str_extended.tell());
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

	istream_extended str_extended(std::move(stream));
	str_extended.seek_forward_head_of_line();
	Assert::IsTrue(expected == str_extended.tell());
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

	istream_extended str_extended(std::move(stream));
	str_extended.seek_forward_head_of_line();
	Assert::IsTrue(expected == str_extended.tell());
}
