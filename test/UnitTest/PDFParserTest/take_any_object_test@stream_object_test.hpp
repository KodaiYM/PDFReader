#pragma once

#include "testtool.h"

namespace pdfparser_test { namespace take_any_object_test {
	[TestClass] public ref class stream_object_test {
	public:
		[TestMethod] void test_sample_CRLF();
		[TestMethod] void test_sample_LF();
		[TestMethod] void test_indirect_reference_Length();
		[TestMethod] void test_stream_CR();
		[TestMethod] void test_absence_of_Length_entry();
		[TestMethod] void test_data_is_shorter_than_Length();
		[TestMethod] void test_keyword_endstream_not_found();
	};
}} // namespace pdfparser_test::take_any_object_test
