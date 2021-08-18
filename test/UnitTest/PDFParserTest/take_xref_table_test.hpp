#pragma once

#include "testtool.h"

namespace pdfparser_test {
[TestClass] public ref class take_xref_table_test {
public:
	[TestMethod] void test_maximum_xref_table();
	[TestMethod] void test_overflow();
};
} // namespace pdfparser_test
