#pragma once

#include "testtool.h"

#include <sstream>

namespace pdfparser_test {
[TestClass] public ref class ignore_if_present_test {
public:
	[TestInitialize] void initialize();
	[TestCleanup] void    cleanup();

public:
	// TODO: ignore_if_present のテストケース増やす（作りかけ）
	[TestMethod] void test_all_whitespaces();

private:
	std::stringstream* m_ss;
};
} // namespace pdfparser_test
