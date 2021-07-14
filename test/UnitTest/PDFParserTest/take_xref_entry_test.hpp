#pragma once

#include "testtool.h"

namespace pdfparser_test {
[TestClass] public ref class take_xref_entry_test {
public:
	[TestInitialize] void initialize();
	[TestCleanup] void    cleanup();

public:
	[TestMethod] void test_SPCR();
	[TestMethod] void test_SPLF();
	[TestMethod] void test_CRLF();

private:
	std::stringstream* m_ss;
};
} // namespace pdfparser_test
