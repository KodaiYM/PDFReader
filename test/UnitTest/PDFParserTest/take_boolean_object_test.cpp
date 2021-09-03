#include "pdfparser.stream_parser.hpp"
#include "take_boolean_object_test.hpp"

#include <sstream>

using namespace pdfparser;
using namespace object_types;
using namespace stream_parser_test::take_object_test;

void take_boolean_object_test::test_true() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "true";

	stream_parser str_parser(std::move(stream));

	auto object = str_parser.take_boolean_object();
	Assert::IsTrue(true == object);
}
void take_boolean_object_test::test_false() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "false";

	stream_parser str_parser(std::move(stream));

	auto object = str_parser.take_boolean_object();
	Assert::IsTrue(false == object);
}
