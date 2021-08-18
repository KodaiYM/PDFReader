#pragma once

#include "testtool.h"

namespace pdfparser_test { namespace take_any_object_test {
	[TestClass] public ref class integer_object_test {
	public:
		[TestMethod] void test_unsigned_integer();
		[TestMethod] void test_plus_integer();
		[TestMethod] void test_minus_integer();
	};
}} // namespace pdfparser_test::take_any_object_test
