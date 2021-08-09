#include "PDFParser.h"
#include "seek_test.hpp"

#include <sstream>

using namespace pdfparser;
using namespace error_types;
using namespace object_types;
using namespace pdfparser_test;

void seek_test::test_to_between_CRLF() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "\r\n"
	       << "\n"
	       << "\r\r"
	       << "\r\n";

	stream_parser str_parser(std::move(stream));
	str_parser.seek(6);
	Assert::IsTrue(6 == str_parser.tell());
}
void seek_test::test_to_EOF() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "test";

	stream_parser str_parser(std::move(stream));
	str_parser.seek(4);
	Assert::IsTrue(4 == str_parser.tell());
}
void seek_test::test_over_EOF() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "test";

	stream_parser str_parser(std::move(stream));
	try {
		str_parser.seek(5);
	} catch (const parse_error& parse_e) {
		Assert::IsTrue(parse_error::failed_to_seek == parse_e.code());

		// success
		return;
	}
	Assert::Fail();
}
