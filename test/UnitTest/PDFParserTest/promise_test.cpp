#include "pdfparser.istream_extended.hpp"
#include "promise_test.hpp"

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

	try {
		str_extended.promise({"e", "d", "c", "b"});
	} catch (const promise_failed& e) {
		Assert::IsTrue(0 == e.tell_position());

		// success
		return;
	}
	Assert::Fail();
}
