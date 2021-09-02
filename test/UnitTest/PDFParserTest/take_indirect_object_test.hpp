#pragma once

#include "testtool.h"

namespace pdfparser_test {
[TestClass] public ref class take_indirect_object_test {
public:
	[TestMethod] void test_sample();
	[TestMethod] void test_inconsistent_object_number();
	[TestMethod] void test_inconsistent_generation_number();
};
} // namespace pdfparser_test
