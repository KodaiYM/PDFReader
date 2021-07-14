#pragma once

#include "testtool.h"

#include <sstream>

namespace pdfparser_test {
// TODO: ‚±‚ê‚ğì‚é
[TestClass] public ref class require_test {
public:
	[TestInitialize] void initialize();
	[TestCleanup] void    cleanup();

public:
	[TestMethod] void test_EOF_success();
	[TestMethod] void test_EOL_fail();
	[TestMethod] void test_startxref_success();
	[TestMethod] void test_startxref_fail();
	[TestMethod] void test_xref_success();
	[TestMethod] void test_xref_fail();
	[TestMethod] void test_space_success();
	[TestMethod] void test_space_fail();

private:
	std::stringstream* ss;
};
} // namespace pdfparser_test
