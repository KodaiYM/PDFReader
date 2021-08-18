#pragma once

#include "testtool.h"

namespace pdfparser_test {
[TestClass] public ref class take_trailer_test {
public:
	[TestMethod] void test_sample_trailer();
	[TestMethod] void test_no_white_space_after_keyword_trailer();
	[TestMethod] void test_white_space_before_keyword_trailer();
	[TestMethod] void test_no_dictionary_object();
	[TestMethod] void test_no_keyword_trailer();
};
} // namespace pdfparser_test
