#include "pdfparser.istream_extended.hpp"
#include "pdfparser.istream_extended_errors.hpp"
#include "seek_forward_head_of_line_test.hpp"

#include <sstream>

using namespace pdfparser;
using namespace istream_extended_test;

/**
 * | abc
 * def �̎��ɃG���[�ɂȂ邩
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
 * | ghi �̎���
 *
 * abc
 * | def
 * ghi �ɂȂ邩
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
 * de | f �̎���
 *
 * abc
 * | def �ɂȂ邩
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
 * CR | �̎���
 * | CR �ɂȂ邩
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
 * LF | �̎���
 * | LF �ɂȂ邩
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
 * CRLF | �̎���
 * | CRLF �ɂȂ邩
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
 * CR | �̎���
 * | CR �ɂȂ邩
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
 * xxx LF | �̎���
 * | xxx LF �ɂȂ邩
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
 * xxx CRLF | �̎���
 * | xxx CRLF �ɂȂ邩
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
 * c CR CR | �̎���
 * c CR | CR �ɂȂ邩
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
 * c LF CR | �̎���
 * c LF | CR �ɂȂ邩
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
 * c LF LF | �̎���
 * c LF | LF �ɂȂ邩
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
 * c CR CRLF | �̎���
 * c CR | CRLF �ɂȂ邩
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
 * c LF CRLF | �̎���
 * c LF | CRLF �ɂȂ邩
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
