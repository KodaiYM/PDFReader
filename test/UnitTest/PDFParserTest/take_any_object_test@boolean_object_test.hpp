#pragma once

#include "testtool.h"

namespace pdfparser_test { namespace take_any_object_test {
	[TestClass] public ref class boolean_object_test {
	public:
		[TestMethod] void test_true();
		[TestMethod] void test_false();
	};
}} // namespace pdfparser_test::take_any_object_test
