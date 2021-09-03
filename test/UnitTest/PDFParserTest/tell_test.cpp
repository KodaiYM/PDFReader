#include "pdfparser.istream_extended.hpp"
#include "tell_test.hpp"

#include <sstream>

using namespace pdfparser;
using namespace istream_extended_test;

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

	istream_extended str_extended(std::move(stream));
	Assert::IsTrue(6 == str_extended.tell());
}
void tell_test::test_tell_at_EOF() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	istream_extended str_extended(std::move(stream));
	Assert::IsTrue(0 == str_extended.tell());
}
void tell_test::test_tell_with_eofbit_ON() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	istream_extended str_extended(std::move(stream));
	(void)str_extended.eof(); // setstate std::ios_base::eofbit
	Assert::IsTrue(0 == str_extended.tell());
}
