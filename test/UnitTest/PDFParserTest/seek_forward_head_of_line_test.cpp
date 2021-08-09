#include "PDFParser.h"
#include "seek_forward_head_of_line_test.hpp"

#include <sstream>

using namespace pdfparser;
using namespace error_types;
using namespace object_types;
using namespace pdfparser_test;

/**
 * | abc
 * def ‚Ì‚ÉƒGƒ‰[‚É‚È‚é‚©
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

	stream_parser str_parser(std::move(stream));
	str_parser.seek_forward_head_of_line();
	Assert::IsTrue(expected == str_parser.tell());
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

	stream_parser str_parser(std::move(stream));
	str_parser.seek_forward_head_of_line();
	Assert::IsTrue(expected == str_parser.tell());
}
/**
 * CR | ‚Ì‚É
 * | CR ‚É‚È‚é‚©
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
 * LF | ‚Ì‚É
 * | LF ‚É‚È‚é‚©
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
 * CRLF | ‚Ì‚É
 * | CRLF ‚É‚È‚é‚©
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

	stream_parser str_parser(std::move(stream));
	str_parser.seek_forward_head_of_line();
	Assert::IsTrue(expected == str_parser.tell());
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

	stream_parser str_parser(std::move(stream));
	str_parser.seek_forward_head_of_line();
	Assert::IsTrue(expected == str_parser.tell());
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

	stream_parser str_parser(std::move(stream));
	str_parser.seek_forward_head_of_line();
	Assert::IsTrue(expected == str_parser.tell());
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

	stream_parser str_parser(std::move(stream));
	str_parser.seek_forward_head_of_line();
	Assert::IsTrue(expected == str_parser.tell());
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

	stream_parser str_parser(std::move(stream));
	str_parser.seek_forward_head_of_line();
	Assert::IsTrue(expected == str_parser.tell());
}
