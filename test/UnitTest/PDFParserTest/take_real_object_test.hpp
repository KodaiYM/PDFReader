#pragma once

#include "testtool.h"

namespace ipdfstream_test::take_object_test {
[TestClass] public ref class take_real_object_test {
public:
	[TestMethod] void test_unsigned_leading_period();
	[TestMethod] void test_unsigned_trailing_period();
	[TestMethod] void test_unsigned_embedded_period();
	[TestMethod] void test_plus_leading_period();
	[TestMethod] void test_plus_trailing_period();
	[TestMethod] void test_plus_embedded_period();
	[TestMethod] void test_minus_leading_period();
	[TestMethod] void test_minus_trailing_period();
	[TestMethod] void test_minus_embedded_period();
};
} // namespace ipdfstream_test::take_object_test
