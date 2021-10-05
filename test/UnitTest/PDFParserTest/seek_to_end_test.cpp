#include "pdfparser.istream_extended.hpp"
#include "seek_to_end_test.hpp"

#include <sstream>

using namespace pdfparser;
using namespace istream_extended_test;

void seek_to_end_test::test_MIX_EOL() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "\r\n"
	       << "\n"
	       << "\r\r";

	istream_extended str_extended(stream.rdbuf());
	str_extended.seek_to_end();
	Assert::IsTrue(5 == str_extended.tell());
}
void seek_to_end_test::test_at_EOF() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	istream_extended str_extended(stream.rdbuf());

	// check if no-throw
	str_extended.seek_to_end();
	str_extended.seek_to_end();
}
