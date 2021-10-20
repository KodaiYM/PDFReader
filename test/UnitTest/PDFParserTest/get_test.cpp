#include "pdfparser.istream_extended.hpp"
#include "pdfparser.istream_extended_errors.hpp"
#include "testtool.h"

namespace istream_extended_test {
[TestClass] public ref class get_test {
public:
	[TestMethod] void test_not_at_EOF();
	[TestMethod] void test_at_EOF();
};
} // namespace istream_extended_test

using namespace pdfparser;
using namespace istream_extended_test;

void get_test::test_not_at_EOF() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "s";
	istream_extended str_extended(stream.rdbuf());
	Assert::IsTrue('s' == str_extended.get());
	Assert::IsTrue(1 == str_extended.tell());
}
void get_test::test_at_EOF() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	istream_extended str_extended(stream.rdbuf());

	try {
		(void)str_extended.get();
	} catch (const failed_to_get& e) {
		Assert::IsTrue(0 == e.tell_position());

		// success
		return;
	}
	Assert::Fail();
}
