#include "pdfparser.stream_parser.hpp"
#include "take_integer_object_test.hpp"

#include <sstream>

using namespace pdfparser;
using namespace object_types;
using namespace stream_parser_test::take_object_test;

void take_integer_object_test::test_unsigned_integer() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "0123";

	document_parser str_parser(std::move(stream));

	integer_object object = str_parser.take_integer_object();
	Assert::AreEqual(123, static_cast<int>(object));
}
void take_integer_object_test::test_plus_integer() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "+0123";

	document_parser str_parser(std::move(stream));

	integer_object object = str_parser.take_integer_object();
	Assert::AreEqual(123, static_cast<int>(object));
}
void take_integer_object_test::test_minus_integer() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "-0123";

	document_parser str_parser(std::move(stream));

	integer_object object = str_parser.take_integer_object();
	Assert::AreEqual(-123, static_cast<int>(object));
}
void take_integer_object_test::test_max() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	constexpr integer_object::int_type max =
	    std::numeric_limits<decltype(max)>::max();
	stream << max;
	std::streamoff expected = stream.tellp();

	document_parser          str_parser(std::move(stream));
	integer_object::int_type integer = str_parser.take_integer_object();
	Assert::AreEqual(max, integer);
}
void take_integer_object_test::test_min() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	constexpr integer_object::int_type min =
	    std::numeric_limits<decltype(min)>::min();
	stream << min;
	std::streamoff expected = stream.tellp();

	document_parser          str_parser(std::move(stream));
	integer_object::int_type integer = str_parser.take_integer_object();
	Assert::AreEqual(min, integer);
}

void take_integer_object_test::test_not_number() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "deadbeef";

	document_parser str_parser(std::move(stream));
	try {
		str_parser.take_integer_object();
	} catch (const object_not_found_error& obj_e) {
		Assert::IsTrue(object_not_found_error::integer_object_not_found ==
		               obj_e.code());

		// success
		return;
	}
	Assert::Fail();
}
void take_integer_object_test::test_EOF() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	document_parser str_parser(std::move(stream));
	try {
		str_parser.take_integer_object();
	} catch (const object_not_found_error& obj_e) {
		Assert::IsTrue(object_not_found_error::integer_object_not_found ==
		               obj_e.code());

		// success
		return;
	}
	Assert::Fail();
}
void take_integer_object_test::test_plus_not_number() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "+deadbeef";

	document_parser str_parser(std::move(stream));
	try {
		str_parser.take_integer_object();
	} catch (const object_not_found_error& obj_e) {
		Assert::IsTrue(object_not_found_error::integer_object_not_found ==
		               obj_e.code());

		// success
		return;
	}
	Assert::Fail();
}
void take_integer_object_test::test_minus_not_number() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "-deadbeef";

	document_parser str_parser(std::move(stream));
	try {
		str_parser.take_integer_object();
	} catch (const object_not_found_error& obj_e) {
		Assert::IsTrue(object_not_found_error::integer_object_not_found ==
		               obj_e.code());

		// success
		return;
	}
	Assert::Fail();
}
void take_integer_object_test::test_sign_EOF() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "+";

	document_parser str_parser(std::move(stream));
	try {
		str_parser.take_integer_object();
	} catch (const object_not_found_error& obj_e) {
		Assert::IsTrue(object_not_found_error::integer_object_not_found ==
		               obj_e.code());

		// success
		return;
	}
	Assert::Fail();
}
void take_integer_object_test::test_max_plus1() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	constexpr auto max = std::numeric_limits<integer_object::int_type>::max();
	stream << max / 10 << ((max % 10) + 1);

	document_parser str_parser(std::move(stream));
	try {
		str_parser.take_integer_object();
	} catch (std::out_of_range&) {
		// success
		return;
	}
	Assert::Fail();
}
void take_integer_object_test::test_min_minus1() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	constexpr auto min = std::numeric_limits<long long>::min();
	stream << min / 10 << (-(min % 10) + 1);

	document_parser str_parser(std::move(stream));
	try {
		str_parser.take_integer_object();
	} catch (std::out_of_range&) {
		// success
		return;
	}
	Assert::Fail();
}
