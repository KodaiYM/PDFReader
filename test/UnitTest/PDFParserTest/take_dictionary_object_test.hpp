#pragma once

#include "testtool.h"

namespace stream_parser_test::take_object_test {
[TestClass] public ref class take_dictionary_object_test {
public:
	[TestMethod] void test_sample();
	[TestMethod] void test_empty();
	[TestMethod] void test_null_value();
	[TestMethod] void test_lack_of_double_greater_than_sign();
};
} // namespace stream_parser_test::take_object_test
