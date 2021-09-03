#pragma once

#include "testtool.h"

namespace stream_parser_test::take_object_test {
[TestClass] public ref class take_literal_string_test {
public:
	[TestMethod] void test_sample();
	[TestMethod] void test_escape_sequence();
	[TestMethod] void test_invalid_escape_sequence();
	[TestMethod] void test_escaped_EOL();
	[TestMethod] void test_EOL_unification();
	[TestMethod] void test_short_octal();
	[TestMethod] void test_octal_overflow();
	[TestMethod] void test_lack_of_right_parenthesis();
};
} // namespace stream_parser_test::take_object_test
