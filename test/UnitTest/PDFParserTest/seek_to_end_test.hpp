#pragma once

#include "testtool.h"

namespace pdfparser_test {
[TestClass] public ref class seek_to_end_test {
public:
	[TestMethod] void test_MIX_EOL();
	[TestMethod] void test_at_EOF();
};
} // namespace pdfparser_test
