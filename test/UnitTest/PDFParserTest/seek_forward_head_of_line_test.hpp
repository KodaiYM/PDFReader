#pragma once

#include "testtool.h"

namespace istream_extended_test {
[TestClass] public ref class seek_forward_head_of_line_test {
public:
	[TestMethod] void test_beginning_of_file();
	[TestMethod] void test_beginning_of_line();
	[TestMethod] void test_middle_of_line();
	[TestMethod] void test_CR_only();
	[TestMethod] void test_LF_only();
	[TestMethod] void test_CRLF_only();
	[TestMethod] void test_CR();
	[TestMethod] void test_LF();
	[TestMethod] void test_CRLF();
	[TestMethod] void test_CR_CRCR();
	[TestMethod] void test_CR_LFCR();
	[TestMethod] void test_LF_LFLF();
	[TestMethod] void test_CRLF_CRCRLF();
	[TestMethod] void test_CRLF_LFCRLF();
};
} // namespace istream_extended_test
