#pragma once

#include "testtool.h"

namespace pdfparser_test {
[TestClass] public ref class take_xref_entry_test {
public:
	[TestMethod] void test_SPCR();
	[TestMethod] void test_SPLF();
	[TestMethod] void test_CRLF();
};
} // namespace pdfparser_test
