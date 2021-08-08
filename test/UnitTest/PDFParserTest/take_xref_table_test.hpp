#pragma once

#include "testtool.h"

#include <sstream>

namespace pdfparser_test {
[TestClass] public ref class take_xref_table_test {
public:
public:
	[TestMethod] void test_maximum_xref_table();
	[TestMethod] void test_overflow();

private:
	pdfparser::stream_parser<std::stringstream>* stream;
};
} // namespace pdfparser_test
