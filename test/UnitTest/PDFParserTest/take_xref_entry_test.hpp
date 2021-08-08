#pragma once

#include "testtool.h"

namespace pdfparser_test {
[TestClass] public ref class take_xref_entry_test {
public:
public:
	[TestMethod] void test_SPCR();
	[TestMethod] void test_SPLF();
	[TestMethod] void test_CRLF();

private:
	pdfparser::stream_parser<std::stringstream>* stream;
};
} // namespace pdfparser_test
