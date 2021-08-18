#include "PDFParser.h"
#include "attempt_test.hpp"

#include <sstream>

using namespace pdfparser;
using namespace error_types;
using namespace object_types;
using namespace pdfparser_test;

using namespace std::string_literals;

void attempt_test::test_match() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "test" << '\0' << " \t\n\f\r "
	       << "test";

	stream_parser str_parser(std::move(stream));

	Assert::IsTrue(str_parser.attempt("test"s + '\0' + " \t\n\f\r " + "test"));
}
void attempt_test::test_fail_during_match() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "\t\n\f\r\n";

	stream_parser str_parser(std::move(stream));

	Assert::IsFalse(str_parser.attempt("\t\n\f\n"));
	Assert::IsTrue(0 == str_parser.tell());
}
void attempt_test::test_empty_match_on_EOF() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream_parser str_parser(std::move(stream));

	Assert::IsTrue(str_parser.attempt(""));
}
void attempt_test::test_non_empty_match_on_EOF() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream_parser str_parser(std::move(stream));

	Assert::IsFalse(str_parser.attempt("test"));
}
