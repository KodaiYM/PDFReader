#pragma once

#include "testtool.h"

#include <sstream>

namespace pdfparser_test {
[TestClass] public ref class take_xref_table_test {
public:
	[TestInitialize] void initialize();
	[TestCleanup] void    cleanup();

public:
	[TestMethod] void test_maximum_xref_table();
	[TestMethod] void test_overflow();

private:
	std::stringstream* m_ss;
};
} // namespace pdfparser_test
