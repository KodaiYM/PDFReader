#include "pdfparser.object_stream.hpp"
#include "pdfparser.object_stream_errors.hpp"
#include "take_hexadecimal_string_test.hpp"

#include <sstream>

using namespace pdfparser;
using namespace object_types;
using namespace object_stream_test;

void take_hexadecimal_string_test::test_mix_letters() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "<0A1B2C3d4e5fe6d7c8b9a1>";

	object_stream obj_stream(stream.rdbuf());
	auto          object = obj_stream.take_string_object();
	Assert::IsTrue("\x0A\x1B\x2C\x3D\x4E\x5F\xE6\xD7\xC8\xB9\xA1" == object);
	Assert::IsTrue(0 == object.position());
}
void take_hexadecimal_string_test::test_whitespace() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "< 20 3A F  5  >";

	object_stream obj_stream(stream.rdbuf());
	auto          object = obj_stream.take_string_object();
	Assert::IsTrue("\x20\x3A\xF5" == object);
	Assert::IsTrue(0 == object.position());
}
void take_hexadecimal_string_test::test_odd_number_of_digits() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "<901FA>";

	object_stream obj_stream(stream.rdbuf());
	auto          object = obj_stream.take_string_object();
	Assert::IsTrue("\x90\x1F\xA0" == object);
	Assert::IsTrue(0 == object.position());
}
void take_hexadecimal_string_test::test_non_hexadecimal_digit_found() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "<901GA>";

	object_stream obj_stream(stream.rdbuf());
	try {
		obj_stream.take_string_object();
	} catch (const hexadecimal_string_non_hexadecimal_digit_found& e) {
		Assert::IsTrue(4 == e.tell_position());

		// success
		return;
	}
	Assert::Fail();
}
void take_hexadecimal_string_test::test_lack_of_greater_than_sign() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "<901FA";

	object_stream obj_stream(stream.rdbuf());
	try {
		obj_stream.take_string_object();
	} catch (const hexadecimal_string_lack_of_greater_than_sign& e) {
		Assert::IsTrue(0 == e.tell_position());

		// success
		return;
	}
	Assert::Fail();
}
