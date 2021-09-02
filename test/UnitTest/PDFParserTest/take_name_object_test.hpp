#pragma once

#include "testtool.h"

namespace pdfparser_test {
[TestClass] public ref class take_name_object_test {
public:
	[TestMethod] void test_valid_name();
	[TestMethod] void test_empty_name();
	[TestMethod] void test_hexadecimal_code();
};
} // namespace pdfparser_test
