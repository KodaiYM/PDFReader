#pragma once

#include "testtool.h"

namespace pdfparser_test {
[TestClass] public ref class peek_test {
public:
	[TestMethod] void test_not_at_EOF();
	[TestMethod] void test_at_EOF();
};
} // namespace pdfparser_test
