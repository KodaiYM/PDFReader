#pragma once

#include "testtool.h"

#include <sstream>

namespace pdfparser_test {
[TestClass] public ref class take_unsigned_integer_test {
public:
	[TestInitialize] void initialize();
	[TestCleanup] void    cleanup();

public:
	// TODO: �e�X�g���\�b�h�����
	[TestMethod] void test_();

private:
	std::stringstream* m_ss;
};
} // namespace pdfparser_test
