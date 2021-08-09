#include "PDFParser.h"
#include "tell_test.hpp"

#include <sstream>

using namespace pdfparser;
using namespace error_types;
using namespace object_types;
using namespace pdfparser_test;

void tell_test::test_MIX_EOL() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "\r\n"
	       << "\n"
	       << "\r\r"
	       << "\r\n";
	stream.ignore();
	stream.ignore();
	stream.ignore();
	stream.ignore();
	stream.ignore();
	stream.ignore();

	stream_parser str_parser(std::move(stream));
	Assert::IsTrue(6 == str_parser.tell());
}
