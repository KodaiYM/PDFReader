#pragma once

#include "testtool.h"

namespace object_parser_test::take_object_test {
[TestClass] public ref class take_hexadecimal_string_test {
public:
	[TestMethod] void test_mix_letters();
	[TestMethod] void test_whitespace();
	[TestMethod] void test_odd_number_of_digits();
	[TestMethod] void test_non_hexadecimal_digit_found();
	[TestMethod] void test_lack_of_greater_than_sign();
};
} // namespace object_parser_test::take_object_test
