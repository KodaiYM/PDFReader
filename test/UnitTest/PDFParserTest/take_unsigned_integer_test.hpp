#pragma once

#include "testtool.h"

#include <sstream>

namespace pdfparser_test {
[TestClass] public ref class take_unsigned_integer_test {
public:
public:
	[TestMethod] void test_unsigned();
	[TestMethod] void test_max();
	[TestMethod] void test_min();

	[TestMethod] void test_not_number();
	[TestMethod] void test_EOF();
	[TestMethod] void test_signed();
	[TestMethod] void test_max_plus1();

private:
	pdfparser::stream_parser<std::stringstream>* stream;
};
} // namespace pdfparser_test
