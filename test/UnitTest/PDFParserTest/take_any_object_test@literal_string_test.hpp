#pragma once

#include "testtool.h"

namespace pdfparser_test { namespace take_any_object_test {
	[TestClass] public ref class literal_string_test {
	public:
		[TestMethod] void test_literal_string_sample();
		[TestMethod] void test_literal_string_escape_sequence();
		[TestMethod] void test_literal_string_invalid_escape_sequence();
		[TestMethod] void test_literal_string_escaped_EOL();
		[TestMethod] void test_literal_string_EOL_unification();
		[TestMethod] void test_literal_string_octal_overflow();
		[TestMethod] void test_literal_string_lack_of_right_parenthesis();
	};
}} // namespace pdfparser_test::take_any_object_test
