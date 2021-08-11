#pragma once

#include "testtool.h"

namespace pdfparser_test { namespace take_any_object_test {
	[TestClass] public ref class dictionary_object_test {
	public:
		[TestMethod] void test_sample();
		[TestMethod] void test_empty();
		[TestMethod] void test_null_value();
		[TestMethod] void test_key_is_not_name_object();
		[TestMethod] void test_lack_of_double_greater_than_sign();
	};
}} // namespace pdfparser_test::take_any_object_test
