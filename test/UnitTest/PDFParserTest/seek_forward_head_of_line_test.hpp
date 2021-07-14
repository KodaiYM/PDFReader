#pragma once

#include "testtool.h"

#include <sstream>

namespace pdfparser_test {
[TestClass] public ref class seek_forward_head_of_line_test {
public:
	[TestInitialize] void initialize();
	[TestCleanup] void    cleanup();

public:
	[TestMethod] void test_beginning_of_file();
	[TestMethod] void test_beginning_of_line();
	[TestMethod] void test_middle_of_line();
	[TestMethod] void test_CR();
	[TestMethod] void test_LF();
	[TestMethod] void test_CRLF();
	[TestMethod] void test_CR_CRCR();
	[TestMethod] void test_CR_LFCR();
	[TestMethod] void test_LF_LFLF();
	[TestMethod] void test_CRLF_CRCRLF();
	[TestMethod] void test_CRLF_LFCRLF();

private:
	std::stringstream* m_ss;
};
} // namespace pdfparser_test
