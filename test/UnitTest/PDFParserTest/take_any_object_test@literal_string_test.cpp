#include "PDFParser.h"
#include "literal_trim.hpp"
#include "take_any_object_test@literal_string_test.hpp"

#include <sstream>

using namespace pdfparser;
using namespace error_types;
using namespace object_types;
using namespace pdfparser_test;

using namespace take_any_object_test;

void literal_string_test::test_sample() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "( String may contain balanced parentheses () and\n"
	          "special characters (*!&}^% and so on). )";

	stream_parser str_parser(std::move(stream));
	object_pool   obj_pool(str_parser);
	auto          object = str_parser.take_any_object(obj_pool);
	Assert::IsTrue(" String may contain balanced parentheses () and\nspecial"
	               " characters (*!&}^% and so on). " ==
	               std::get<string_object>(object));
}
void literal_string_test::test_escape_sequence() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << R"((\n\r\r\n\t\b\f\(\)\\\123))";

	stream_parser str_parser(std::move(stream));
	object_pool   obj_pool(str_parser);
	auto          object = str_parser.take_any_object(obj_pool);
	Assert::IsTrue("\n\r\r\n\t\b\f()\\\123" == std::get<string_object>(object));
}
void literal_string_test::test_invalid_escape_sequence() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "(\\a)";

	stream_parser str_parser(std::move(stream));
	object_pool   obj_pool(str_parser);
	auto          object = str_parser.take_any_object(obj_pool);
	Assert::IsTrue("a" == std::get<string_object>(object));
}
void literal_string_test::test_escaped_EOL() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << R"(
(These \
two strings \
are the same.)
)"_trimmed;

	stream_parser str_parser(std::move(stream));
	object_pool   obj_pool(str_parser);
	auto          object = str_parser.take_any_object(obj_pool);
	Assert::IsTrue("These two strings are the same." ==
	               std::get<string_object>(object));
}
void literal_string_test::test_EOL_unification() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "(\n\r\n\r)";

	stream_parser str_parser(std::move(stream));
	object_pool   obj_pool(str_parser);
	auto          object = str_parser.take_any_object(obj_pool);
	Assert::IsTrue("\n\n\n" == std::get<string_object>(object));
}
void literal_string_test::test_octal_overflow() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "(\\777)";

	stream_parser str_parser(std::move(stream));
	object_pool   obj_pool(str_parser);
	auto          object = str_parser.take_any_object(obj_pool);
	Assert::IsTrue("\377" == std::get<string_object>(object));
}
void literal_string_test::test_lack_of_right_parenthesis() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "(abc(\\)def)";

	stream_parser str_parser(std::move(stream));
	object_pool   obj_pool(str_parser);
	try {
		str_parser.take_any_object(obj_pool);
	} catch (const parse_error& parse_e) {
		Assert::IsTrue(parse_error::literal_string_lack_of_right_parenthesis ==
		               parse_e.code());

		// success
		return;
	}
	Assert::Fail();
}
