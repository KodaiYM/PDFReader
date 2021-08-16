#include "PDFParser.h"
#include "take_any_object_test@name_object_test.hpp"

#include <sstream>
#include <string>

using namespace pdfparser;
using namespace error_types;
using namespace object_types;
using namespace pdfparser_test;

using namespace take_any_object_test;
using namespace std::string_literals;

void name_object_test::test_valid_name() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << R"(/;*_-.$@!"&'=~^\|`+:,?)";

	stream_parser str_parser(std::move(stream));
	object_pool   obj_pool(str_parser);
	auto          object = str_parser.take_any_object(obj_pool);
	Assert::IsTrue(R"(;*_-.$@!"&'=~^\|`+:,?)" == std::get<name_object>(object));
}
void name_object_test::test_empty_name() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "/";

	stream_parser str_parser(std::move(stream));
	object_pool   obj_pool(str_parser);
	auto          object = str_parser.take_any_object(obj_pool);
	Assert::IsTrue("" == std::get<name_object>(object));
}
void name_object_test::test_hexadecimal_code() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "/A#20B";

	stream_parser str_parser(std::move(stream));
	object_pool   obj_pool(str_parser);
	auto          object = str_parser.take_any_object(obj_pool);
	Assert::IsTrue("A"s + '\x20' + "B" == std::get<name_object>(object));
}
void name_object_test::test_invalid_hexadecimal_code() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "/#2Z";

	stream_parser str_parser(std::move(stream));
	object_pool   obj_pool(str_parser);

	try {
		str_parser.take_any_object(obj_pool);
	} catch (const parse_error& parse_e) {
		Assert::IsTrue(parse_error::name_invalid_hexadecimal_code ==
		               parse_e.code());

		// success
		return;
	}
	Assert::Fail();
}
