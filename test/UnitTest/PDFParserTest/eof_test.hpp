#pragma once

#include "testtool.h"

namespace pdfparser_test {
[TestClass] public ref class eof_test {
public:
	[TestMethod] void test_not_at_EOF();
	[TestMethod] void test_at_EOF();
	[TestMethod] void test_with_eofbit_ON();
};
} // namespace pdfparser_test
