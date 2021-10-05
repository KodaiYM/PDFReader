#include "pdfparser.ipdfstream.hpp"
#include "take_name_object_test.hpp"

#include <sstream>
#include <string>

using namespace pdfparser;
using namespace object_types;
using namespace ipdfstream_test::take_object_test;

using namespace std::string_literals;

void take_name_object_test::test_valid_name() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << R"(/;*_-.$@!"&'=~^\|`+:,?)";

	ipdfstream str_parser(stream.rdbuf());

	auto object = str_parser.take_name_object();
	Assert::IsTrue(R"(;*_-.$@!"&'=~^\|`+:,?)" == object);
}
void take_name_object_test::test_empty_name() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "/";

	ipdfstream str_parser(stream.rdbuf());

	auto object = str_parser.take_name_object();
	Assert::IsTrue("" == object);
}
void take_name_object_test::test_hexadecimal_code() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "/A#20B";

	ipdfstream str_parser(stream.rdbuf());

	auto object = str_parser.take_name_object();
	Assert::IsTrue("A"s + '\x20' + "B" == object);
}
