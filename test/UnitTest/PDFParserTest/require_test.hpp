#pragma once

#include "testtool.h"

#include <sstream>

namespace pdfparser_test {
[TestClass] public ref class require_test {
public:
	[TestInitialize] void initialize();
	[TestCleanup] void    cleanup();

public:
	[TestMethod] void test_EOF_EOF_EOL();
	[TestMethod] void test_EOF_EOF_only();
	[TestMethod] void test_EOF_EOF_not_EOL();
	[TestMethod] void test_EOF_not_EOF();
	[TestMethod] void test_EOF_SP_EOF();
	[TestMethod] void test_EOF_end_of_line();

	[TestMethod] void test_startxref_startxref_EOL();
	[TestMethod] void test_startxref_SP_startxref_comment_EOL();
	[TestMethod] void test_startxref_startxref_only();
	[TestMethod] void test_startxref_startxref_not_EOL();
	[TestMethod] void test_startxref_not_startxref();
	[TestMethod] void test_startxref_end_of_line();

	[TestMethod] void test_xref_xref_EOL();
	[TestMethod] void test_xref_SP_xref_comment_EOL();
	[TestMethod] void test_xref_xref_only();
	[TestMethod] void test_xref_xref_not_EOL();
	[TestMethod] void test_xref_not_xref();
	[TestMethod] void test_xref_end_of_line();

	[TestMethod] void test_space_space_only();
	[TestMethod] void test_space_not_space();
	[TestMethod] void test_space_end_of_line();

private:
	std::stringstream* m_ss;
};
} // namespace pdfparser_test
