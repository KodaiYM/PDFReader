#pragma once

#include "testtool.h"

namespace stream_parser_test::take_object_test {
[TestClass] public ref class take_integer_object_test {
public:
	[TestMethod] void test_unsigned_integer();
	[TestMethod] void test_plus_integer();
	[TestMethod] void test_minus_integer();
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
} // namespace stream_parser_test::take_object_test
