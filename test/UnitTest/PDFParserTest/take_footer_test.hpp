#pragma once

#include "testtool.h"

namespace pdfparser_test {
[TestClass] public ref class take_footer_test {
public:
	[TestMethod] void test_sample();

	[TestMethod] void test_EOF_EOF_EOL();
	[TestMethod] void test_EOF_EOF_only();
	[TestMethod] void test_EOF_EOF_not_EOL();
	[TestMethod] void test_EOF_not_EOF();
	[TestMethod] void test_EOF_SP_EOF();

	[TestMethod] void test_xref_byte_offset_not_found();

	[TestMethod] void test_startxref_SP_startxref_comment_EOL();
	[TestMethod] void test_startxref_startxref_not_EOL();
	[TestMethod] void test_startxref_not_startxref();

	[TestMethod] void test_xref_SP_xref_comment_EOL();
	[TestMethod] void test_xref_xref_not_EOL();
	[TestMethod] void test_xref_not_xref();
};
} // namespace pdfparser_test
