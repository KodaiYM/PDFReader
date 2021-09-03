#pragma once

#include "testtool.h"

namespace istream_extended_test {
[TestClass] public ref class tell_test {
public:
	[TestMethod] void test_MIX_EOL();
	[TestMethod] void test_tell_at_EOF();
	[TestMethod] void test_tell_with_eofbit_ON();
};
} // namespace istream_extended_test
