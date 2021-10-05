#include "literal_trim.hpp"
#include "pdfparser.ipdfstream.hpp"
#include "take_literal_string_test.hpp"

#include <sstream>

using namespace pdfparser;
using namespace object_types;
using namespace object_parser_test::take_object_test;

void take_literal_string_test::test_sample() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "( String may contain balanced parentheses () and\n"
	          "special characters (*!&}^% and so on). )";

	object_parser str_parser(std::move(stream));

	auto object = str_parser.take_string_object();
	Assert::IsTrue(" String may contain balanced parentheses () and\nspecial"
	               " characters (*!&}^% and so on). " == object);
}
void take_literal_string_test::test_escape_sequence() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << R"((\n\r\r\n\t\b\f\(\)\\\123))";

	object_parser str_parser(std::move(stream));

	auto object = str_parser.take_string_object();
	Assert::IsTrue("\n\r\r\n\t\b\f()\\\123" == object);
}
void take_literal_string_test::test_invalid_escape_sequence() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "(\\a)";

	object_parser str_parser(std::move(stream));

	auto object = str_parser.take_string_object();
	Assert::IsTrue("a" == object);
}
void take_literal_string_test::test_escaped_EOL() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << R"(
(These \
two strings \
are the same.)
)"_trimmed;

	object_parser str_parser(std::move(stream));

	auto object = str_parser.take_string_object();
	Assert::IsTrue("These two strings are the same." == object);
}
void take_literal_string_test::test_EOL_unification() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "(\n\r\n\r)";

	object_parser str_parser(std::move(stream));

	auto object = str_parser.take_string_object();
	Assert::IsTrue("\n\n\n" == object);
}
void take_literal_string_test::test_short_octal() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "(before\\56after)";

	object_parser str_parser(std::move(stream));

	auto object = str_parser.take_string_object();
	Assert::IsTrue("before\56after" == object);
}
void take_literal_string_test::test_octal_overflow() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "(\\777)";

	object_parser str_parser(std::move(stream));

	auto object = str_parser.take_string_object();
	Assert::IsTrue("\377" == object);
}
void take_literal_string_test::test_lack_of_right_parenthesis() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "(abc(\\)def)";

	object_parser str_parser(std::move(stream));

	try {
		str_parser.take_string_object();
	} catch (const parse_error& parse_e) {
		Assert::IsTrue(parse_error::literal_string_lack_of_right_parenthesis ==
		               parse_e.code());

		// success
		return;
	}
	Assert::Fail();
}
