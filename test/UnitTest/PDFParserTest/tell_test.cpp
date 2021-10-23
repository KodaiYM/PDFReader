#include "testtool.h"

namespace istream_extended_test {
[TestClass] public ref class tell_test {
public:
	[TestMethod] void test_MIX_EOL();
	[TestMethod] void test_tell_at_EOF();
	[TestMethod] void test_tell_with_eofbit_ON();
};
} // namespace istream_extended_test

#include "pdfparser.istream_extended.hpp"

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

	istream_extended str_extended(stream.rdbuf());
	Assert::IsTrue(6 == str_extended.tell());
}
void tell_test::test_tell_at_EOF() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	istream_extended str_extended(stream.rdbuf());
	Assert::IsTrue(0 == str_extended.tell());
}
void tell_test::test_tell_with_eofbit_ON() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	istream_extended str_extended(stream.rdbuf());
	(void)str_extended.eof(); // setstate std::ios_base::eofbit
	Assert::IsTrue(0 == str_extended.tell());
}
