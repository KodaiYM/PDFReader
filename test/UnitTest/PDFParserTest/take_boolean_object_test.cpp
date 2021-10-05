#include "pdfparser.ipdfstream.hpp"
#include "take_boolean_object_test.hpp"

#include <sstream>

using namespace pdfparser;
using namespace object_types;
using namespace ipdfstream_test::take_object_test;

void take_boolean_object_test::test_true() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "true";

	ipdfstream str_parser(stream.rdbuf());

	auto object = str_parser.take_boolean_object();
	Assert::IsTrue(true == object);
}
void take_boolean_object_test::test_false() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "false";

	ipdfstream str_parser(stream.rdbuf());

	auto object = str_parser.take_boolean_object();
	Assert::IsTrue(false == object);
}
