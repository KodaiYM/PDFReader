#pragma once

#include "testtool.h"

namespace pdfparser_test { namespace take_any_object_test {
	[TestClass] public ref class name_object_test {
	public:
		[TestMethod] void test_valid_name();
		[TestMethod] void test_empty_name();
		[TestMethod] void test_hexadecimal_code();
		[TestMethod] void test_invalid_hexadecimal_code();
	};
}} // namespace pdfparser_test::take_any_object_test
