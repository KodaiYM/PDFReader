#include "pdfparser.istream_extended.hpp"
#include "peek_test.hpp"

#include <sstream>

using namespace pdfparser;
using namespace istream_extended_test;

void peek_test::test_not_at_EOF() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "s";
	istream_extended str_extended(stream.rdbuf());
	Assert::IsTrue('s' == str_extended.peek());
}
void peek_test::test_at_EOF() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	istream_extended str_extended(stream.rdbuf());
	Assert::IsTrue(!str_extended.peek().has_value());
}
