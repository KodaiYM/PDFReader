#pragma once

#include "testtool.h"

namespace pdfparser_test {
[TestClass] public ref class take_indirect_object_test {
public:
	[TestMethod] void test_sample();
	[TestMethod] void test_inconsistent_object_number();
	[TestMethod] void test_inconsistent_generation_number();
	[TestMethod] void test_keyword_obj_not_found();
	[TestMethod] void test_refers_indirect_reference();
	[TestMethod] void test_keyword_endobj_not_found();
};
} // namespace pdfparser_test
