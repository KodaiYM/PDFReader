#pragma once

#include "testtool.h"

namespace object_stream_test {
[TestClass] public ref class take_object_test {
public:
	[TestMethod] void test_any_direct_object_boolean();
	[TestMethod] void test_any_direct_object_integer();
	[TestMethod] void test_any_direct_object_real();
	[TestMethod] void test_any_direct_object_string();
	[TestMethod] void test_any_direct_object_name();
	[TestMethod] void test_any_direct_object_array();
	[TestMethod] void test_any_direct_object_dictionary();
	[TestMethod] void test_any_direct_object_stream();
	[TestMethod] void test_any_direct_object_null();
	[TestMethod] void test_any_direct_object_indirect_reference();

	[TestMethod] void test_no_object();
	[TestMethod] void test_eof();
};
} // namespace object_stream_test
