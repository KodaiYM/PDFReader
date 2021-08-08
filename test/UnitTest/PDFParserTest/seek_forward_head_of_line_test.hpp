#pragma once

#include "PDFParser.stream_parser.h"
#include "testtool.h"

#include <sstream>

namespace pdfparser_test {
[TestClass] public ref class seek_forward_head_of_line_test {
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
	pdfparser::stream_parser<std::stringstream>* stream;
};
} // namespace pdfparser_test
