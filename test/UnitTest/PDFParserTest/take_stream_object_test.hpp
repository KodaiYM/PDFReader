#pragma once

#include "testtool.h"

namespace object_stream_test {
[TestClass] public ref class take_stream_object_test {
public:
	[TestMethod] void test_sample_CRLF();
	[TestMethod] void test_sample_LF();
	[TestMethod] void test_indirect_reference_Length();
	[TestMethod] void test_absence_of_Length_entry();
	[TestMethod] void test_data_is_shorter_than_Length();
};
} // namespace object_stream_test
