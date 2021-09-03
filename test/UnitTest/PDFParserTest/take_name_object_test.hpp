#pragma once

#include "testtool.h"

namespace stream_parser_test::take_object_test {
[TestClass] public ref class take_name_object_test {
public:
	[TestMethod] void test_valid_name();
	[TestMethod] void test_empty_name();
	[TestMethod] void test_hexadecimal_code();
};
} // namespace stream_parser_test::take_object_test
