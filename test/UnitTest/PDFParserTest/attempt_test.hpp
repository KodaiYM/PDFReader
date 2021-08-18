#pragma once

#include "testtool.h"

namespace pdfparser_test {
[TestClass] public ref class attempt_test {
public:
	[TestMethod] void test_match();
	[TestMethod] void test_fail_during_match();
	[TestMethod] void test_empty_match_on_EOF();
	[TestMethod] void test_non_empty_match_on_EOF();
};
} // namespace pdfparser_test
