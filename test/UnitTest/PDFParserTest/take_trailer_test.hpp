#pragma once

#include "testtool.h"

namespace ipdfstream_test {
[TestClass] public ref class take_trailer_test {
public:
	[TestMethod] void test_sample_trailer();
	[TestMethod] void test_no_white_space_after_keyword_trailer();
};
} // namespace ipdfstream_test
