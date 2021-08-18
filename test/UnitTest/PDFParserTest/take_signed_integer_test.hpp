#pragma once

#include "testtool.h"

namespace pdfparser_test {
[TestClass] public ref class take_signed_integer_test {
public:
	[TestMethod] void test_unsigned();
	[TestMethod] void test_plus();
	[TestMethod] void test_minus();
	[TestMethod] void test_max();
	[TestMethod] void test_min();

	[TestMethod] void test_not_number();
	[TestMethod] void test_EOF();
	[TestMethod] void test_plus_not_number();
	[TestMethod] void test_minus_not_number();
	[TestMethod] void test_sign_EOF();
	[TestMethod] void test_max_plus1();
	[TestMethod] void test_min_minus1();
};
} // namespace pdfparser_test
