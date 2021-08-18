#pragma once

#include "testtool.h"

namespace pdfparser_test {
[TestClass] public ref class take_unsigned_integer_test {
public:
	[TestMethod] void test_unsigned();
	[TestMethod] void test_max();
	[TestMethod] void test_min();

	[TestMethod] void test_not_number();
	[TestMethod] void test_EOF();
	[TestMethod] void test_signed();
	[TestMethod] void test_max_plus1();
};
} // namespace pdfparser_test
