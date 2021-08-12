#pragma once

#include "testtool.h"

namespace pdfparser_test { namespace take_any_object_test {
	[TestClass] public ref class indirect_reference_test {
	public:
		[TestMethod] void test_unsigned();
		[TestMethod] void test_signed();
	};
}} // namespace pdfparser_test::take_any_object_test
