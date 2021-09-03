#pragma once

#include "testtool.h"

namespace istream_extended_test {
[TestClass] public ref class seek_test {
public:
	[TestMethod] void test_to_between_CRLF();
	[TestMethod] void test_to_EOF();
	[TestMethod] void test_over_EOF();
};
} // namespace istream_extended_test
