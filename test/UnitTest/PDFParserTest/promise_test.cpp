#include "testtool.h"

namespace istream_extended_test {
[TestClass] public ref class promise_test {
public:
	[TestMethod] void test_success();
	[TestMethod] void test_failure();
};
} // namespace istream_extended_test

#include "AssertThrows.hpp"
#include "pdfparser.istream_extended.hpp"

using namespace pdfparser;
using namespace istream_extended_test;

void promise_test::test_success() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "abcde";

	istream_extended str_extended(stream.rdbuf());

	str_extended.promise({"e", "d", "c", "b", "a"});
	Assert::IsTrue(1 == str_extended.tell());
}
void promise_test::test_failure() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "abcde";

	istream_extended str_extended(stream.rdbuf());

	AssertThrows(promise_failed, str_extended.promise({"e", "d", "c", "b"}));
}
