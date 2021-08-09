#include "PDFParser.h"
#include "seek_forward_head_of_line_test.hpp"

#include <sstream>

using namespace pdfparser;
using namespace error_types;
using namespace object_types;
using namespace pdfparser_test;

/**
 * | abc
 * def �̎��ɃG���[�ɂȂ邩
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

	stream_parser str_parser(std::move(stream));
	str_parser.seek_forward_head_of_line();
	Assert::IsTrue(expected == str_parser.tell());
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

	stream_parser str_parser(std::move(stream));
	str_parser.seek_forward_head_of_line();
	Assert::IsTrue(expected == str_parser.tell());
}
/**
 * CR | �̎���
 * | CR �ɂȂ邩
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
 * LF | �̎���
 * | LF �ɂȂ邩
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
 * CRLF | �̎���
 * | CRLF �ɂȂ邩
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

	stream_parser str_parser(std::move(stream));
	str_parser.seek_forward_head_of_line();
	Assert::IsTrue(expected == str_parser.tell());
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

	stream_parser str_parser(std::move(stream));
	str_parser.seek_forward_head_of_line();
	Assert::IsTrue(expected == str_parser.tell());
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

	stream_parser str_parser(std::move(stream));
	str_parser.seek_forward_head_of_line();
	Assert::IsTrue(expected == str_parser.tell());
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

	stream_parser str_parser(std::move(stream));
	str_parser.seek_forward_head_of_line();
	Assert::IsTrue(expected == str_parser.tell());
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

	stream_parser str_parser(std::move(stream));
	str_parser.seek_forward_head_of_line();
	Assert::IsTrue(expected == str_parser.tell());
}
