#pragma once

#include "testtool.h"

namespace pdfparser_test {
[TestClass] public ref class tell_test {
public:
	[TestMethod] void test_MIX_EOL();
	[TestMethod] void test_tell_at_EOF();
	[TestMethod] void test_tell_with_eofbit_ON();
};
} // namespace pdfparser_test
