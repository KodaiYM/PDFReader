#pragma once

#include "testtool.h"

namespace ipdfstream_test {
[TestClass] public ref class take_xref_table_test {
public:
	[TestMethod] void test_maximum_xref_table();
	[TestMethod] void test_overflow();
};
} // namespace ipdfstream_test
