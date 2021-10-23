#include "testtool.h"

namespace istream_extended_test {
[TestClass] public ref class eof_test {
public:
	[TestMethod] void test_not_at_EOF();
	[TestMethod] void test_at_EOF();
	[TestMethod] void test_with_eofbit_ON();
};
} // namespace istream_extended_test

#include "pdfparser.istream_extended.hpp"

#include <sstream>

using namespace pdfparser;
using namespace istream_extended_test;

void eof_test::test_not_at_EOF() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "s";
	istream_extended str_extended(stream.rdbuf());
	Assert::IsFalse(str_extended.eof());
}
void eof_test::test_at_EOF() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	istream_extended str_extended(stream.rdbuf());
	Assert::IsTrue(str_extended.eof());
}
void eof_test::test_with_eofbit_ON() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	istream_extended str_extended(stream.rdbuf());
	(void)str_extended.eof(); // setstate std::ios_base::eofbit
	Assert::IsTrue(str_extended.eof());
}
