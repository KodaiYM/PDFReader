#pragma once

#include "testtool.h"

namespace istream_extended_test {
[TestClass] public ref class seek_to_end_test {
public:
	[TestMethod] void test_MIX_EOL();
	[TestMethod] void test_at_EOF();
};
} // namespace istream_extended_test
