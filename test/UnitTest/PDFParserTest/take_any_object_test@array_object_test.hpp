#pragma once

#include "testtool.h"

namespace pdfparser_test { namespace take_any_object_test {
	[TestClass] public ref class array_object_test {
	public:
		[TestMethod] void test_simple_array();
		[TestMethod] void test_valid_array();
		[TestMethod] void test_empty_array();
		[TestMethod] void test_lack_of_right_square_bracket();
	};
}} // namespace pdfparser_test::take_any_object_test
