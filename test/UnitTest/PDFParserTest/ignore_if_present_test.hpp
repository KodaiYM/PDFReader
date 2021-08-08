#pragma once

#include "testtool.h"

#include <sstream>

namespace pdfparser_test {
[TestClass] public ref class ignore_if_present_test {
public:
	[TestMethod] void test_null_only();
	[TestMethod] void test_line_feed_only();
	[TestMethod] void test_form_feed_only();
	[TestMethod] void test_carriage_return_only();
	[TestMethod] void test_space_only();
	[TestMethod] void test_comment_only();
	[TestMethod] void test_EOL_only();
	[TestMethod] void test_any_whitespace_characters();
	[TestMethod] void test_all_whitespaces_including_comment();
	[TestMethod] void test_nothing_to_ignore();
	[TestMethod] void test_no_flags();

private:
	pdfparser::stream_parser<std::stringstream>* stream;
};
} // namespace pdfparser_test
