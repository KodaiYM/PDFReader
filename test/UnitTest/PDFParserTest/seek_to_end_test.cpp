#include "PDFParser.h"
#include "seek_to_end_test.hpp"

#include <sstream>

using namespace pdfparser;
using namespace error_types;
using namespace object_types;
using namespace pdfparser_test;

void seek_to_end_test::test_MIX_EOL() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "\r\n"
	       << "\n"
	       << "\r\r";

	stream_parser str_parser(std::move(stream));
	str_parser.seek_to_end();
	Assert::IsTrue(5 == str_parser.tell());
}
void seek_to_end_test::test_at_EOF() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream_parser str_parser(std::move(stream));
	try {
		str_parser.seek_to_end();
		str_parser.seek_to_end();

		// success
		return;
	} catch (...) {}
	Assert::Fail();
}
