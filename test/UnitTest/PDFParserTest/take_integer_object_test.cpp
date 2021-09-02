#include "pdfparser.stream_parser.hpp"
#include "take_integer_object_test.hpp"

#include <sstream>

using namespace pdfparser;
using namespace object_types;
using namespace pdfparser_test;

void take_integer_object_test::test_unsigned_integer() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "0123";

	stream_parser str_parser(std::move(stream));

	auto object = str_parser.take_integer_object();
	Assert::IsTrue(123 == object);
}
void take_integer_object_test::test_plus_integer() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "+0123";

	stream_parser str_parser(std::move(stream));

	auto object = str_parser.take_integer_object();
	Assert::IsTrue(123 == object);
}
void take_integer_object_test::test_minus_integer() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "-0123";

	stream_parser str_parser(std::move(stream));

	auto object = str_parser.take_integer_object();
	Assert::IsTrue(-123 == object);
}
