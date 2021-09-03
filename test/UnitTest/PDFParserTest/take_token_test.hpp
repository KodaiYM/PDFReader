#pragma once

#include "testtool.h"

namespace tokenizer_test {
[TestClass] public ref class take_token_test {
public:
	[TestMethod] void test_sp_separated_regular_token();
	[TestMethod] void test_eof_separated_regular_token();
	[TestMethod] void test_delimiter_separated_regular_token();

	[TestMethod] void test_double_delimiter_token();
	[TestMethod] void test_single_delimiter_token();

	[TestMethod] void test_comment_ignorable();

	[TestMethod] void test_whitespace_eof();
	[TestMethod] void test_at_eof();
};
} // namespace tokenizer_test
