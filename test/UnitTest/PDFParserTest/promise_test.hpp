#pragma once

#include "testtool.h"

namespace istream_extended_test {
[TestClass] public ref class promise_test {
public:
	[TestMethod] void test_success();
	[TestMethod] void test_failure();
};
} // namespace istream_extended_test
