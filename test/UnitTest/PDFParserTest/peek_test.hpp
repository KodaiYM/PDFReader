#pragma once

#include "testtool.h"

namespace istream_extended_test {
[TestClass] public ref class peek_test {
public:
	[TestMethod] void test_not_at_EOF();
	[TestMethod] void test_at_EOF();
};
} // namespace istream_extended_test
