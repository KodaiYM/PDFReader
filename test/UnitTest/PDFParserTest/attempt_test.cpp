#include "attempt_test.hpp"
#include "pdfparser.istream_extended.hpp"

#include <sstream>

using namespace pdfparser;
using namespace pdfparser_test;

using namespace std::string_literals;

void attempt_test::test_match() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "test" << '\0' << " \t\n\f\r "
	       << "test";

	istream_extended str_extended(std::move(stream));

	Assert::IsTrue(str_extended.attempt("test"s + '\0' + " \t\n\f\r " + "test"));
}
void attempt_test::test_fail_during_match() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "\t\n\f\r\n";

	istream_extended str_extended(std::move(stream));

	Assert::IsFalse(str_extended.attempt("\t\n\f\n"));
	Assert::IsTrue(0 == str_extended.tell());
}
void attempt_test::test_empty_match_on_EOF() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	istream_extended str_extended(std::move(stream));

	Assert::IsTrue(str_extended.attempt(""));
}
void attempt_test::test_non_empty_match_on_EOF() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	istream_extended str_extended(std::move(stream));

	Assert::IsFalse(str_extended.attempt("test"));
}
