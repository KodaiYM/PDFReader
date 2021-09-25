#include "pdfparser.document_parser.hpp"
#include "take_hexadecimal_string_test.hpp"

#include <sstream>

using namespace pdfparser;
using namespace object_types;
using namespace stream_parser_test::take_object_test;

void take_hexadecimal_string_test::test_mix_letters() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "<0A1B2C3d4e5fe6d7c8b9a1>";

	document_parser str_parser(std::move(stream));
	auto            object = str_parser.take_string_object();
	Assert::IsTrue("\x0A\x1B\x2C\x3D\x4E\x5F\xE6\xD7\xC8\xB9\xA1" == object);
}
void take_hexadecimal_string_test::test_whitespace() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "< 20 3A F  5  >";

	document_parser str_parser(std::move(stream));
	auto            object = str_parser.take_string_object();
	Assert::IsTrue("\x20\x3A\xF5" == object);
}
void take_hexadecimal_string_test::test_odd_number_of_digits() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "<901FA>";

	document_parser str_parser(std::move(stream));
	auto            object = str_parser.take_string_object();
	Assert::IsTrue("\x90\x1F\xA0" == object);
}
void take_hexadecimal_string_test::test_non_hexadecimal_digit_found() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "<901GA>";

	document_parser str_parser(std::move(stream));
	try {
		str_parser.take_string_object();
	} catch (const parse_error& parse_e) {
		Assert::IsTrue(
		    parse_error::hexadecimal_string_non_hexadecimal_digit_found ==
		    parse_e.code());

		// success
		return;
	}
	Assert::Fail();
}
void take_hexadecimal_string_test::test_lack_of_greater_than_sign() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "<901FA";

	document_parser str_parser(std::move(stream));
	try {
		str_parser.take_string_object();
	} catch (const parse_error& parse_e) {
		Assert::IsTrue(parse_error::hexadecimal_string_lack_of_greater_than_sign ==
		               parse_e.code());

		// success
		return;
	}
	Assert::Fail();
}
