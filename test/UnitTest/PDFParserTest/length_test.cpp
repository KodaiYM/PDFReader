#include "testtool.h"

namespace istream_extended_test {
[TestClass] public ref class length_test {
public:
	[TestMethod] void test_empty();
	[TestMethod] void test_normal();
};
} // namespace istream_extended_test

#include "pdfparser.istream_extended.hpp"

using namespace pdfparser;
using namespace istream_extended_test;

void length_test::test_empty() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	istream_extended str_extended(stream.rdbuf());
	Assert::IsTrue(0 == str_extended.length());
}
void length_test::test_normal() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "12345";

	istream_extended str_extended(stream.rdbuf());
	Assert::IsTrue(5 == str_extended.length());
}
