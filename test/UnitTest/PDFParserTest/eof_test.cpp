#include "eof_test.hpp"
#include "pdfparser.istream_extended.hpp"

#include <sstream>

using namespace pdfparser;
using namespace pdfparser_test;

void eof_test::test_not_at_EOF() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "s";
	istream_extended str_extended(std::move(stream));
	Assert::IsFalse(str_extended.eof());
}
void eof_test::test_at_EOF() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	istream_extended str_extended(std::move(stream));
	Assert::IsTrue(str_extended.eof());
}
void eof_test::test_with_eofbit_ON() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	istream_extended str_extended(std::move(stream));
	(void)str_extended.eof(); // setstate std::ios_base::eofbit
	Assert::IsTrue(str_extended.eof());
}
