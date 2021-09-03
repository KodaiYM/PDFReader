#pragma once

#include "testtool.h"

namespace tokenizer_test {
[TestClass] public ref class promise_token_test {
public:
	[TestMethod] void test_when_nothrow();
	[TestMethod] void test_when_throw();
};
} // namespace tokenizer_test
