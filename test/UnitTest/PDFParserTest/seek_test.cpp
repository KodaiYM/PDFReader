#include "AssertThrows.hpp"
#include "pdfparser.istream_extended.hpp"
#include "pdfparser.istream_extended_errors.hpp"
#include "seek_test.hpp"

#include <sstream>

using namespace pdfparser;
using namespace istream_extended_test;

void seek_test::test_to_between_CRLF() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "\r\n"
	       << "\n"
	       << "\r\r"
	       << "\r\n";

	istream_extended str_extended(stream.rdbuf());
	str_extended.seek(6);
	Assert::IsTrue(6 == str_extended.tell());
}
void seek_test::test_to_EOF() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "test";

	istream_extended str_extended(stream.rdbuf());
	str_extended.seek(4);
	Assert::IsTrue(4 == str_extended.tell());
}
void seek_test::test_over_EOF() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "test";

	istream_extended str_extended(stream.rdbuf());
	AssertThrows(failed_to_seek, str_extended.seek(5));
}
