#pragma once

#include "testtool.h"

namespace object_parser_test {
[TestClass] public ref class take_footer_test {
public:
	[TestMethod] void test_sample();
	[TestMethod] void test_EOF_EOF_EOL();
	[TestMethod] void test_EOF_EOF_only();

	[TestMethod] void test_xref_byte_offset_not_found();
	[TestMethod] void test_startxref_SP_startxref_comment_EOL();
	[TestMethod] void test_xref_SP_xref_comment_EOL();
};
} // namespace object_parser_test
