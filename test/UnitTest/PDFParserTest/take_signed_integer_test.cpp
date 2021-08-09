#include "PDFParser.h"
#include "take_signed_integer_test.hpp"

#include <sstream>

using namespace pdfparser;
using namespace error_types;
using namespace object_types;
using namespace pdfparser_test;

void take_signed_integer_test::test_unsigned() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "100";
	std::streamoff expected = stream.tellp();

	stream_parser str_parser(std::move(stream));
	auto          integer = str_parser.take_signed_integer<long long>();
	Assert::AreEqual((long long)100, integer);
	Assert::IsTrue(expected == str_parser.tell());
}
void take_signed_integer_test::test_plus() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "+100";
	std::streamoff expected = stream.tellp();

	stream_parser str_parser(std::move(stream));
	auto          integer = str_parser.take_signed_integer<long long>();
	Assert::AreEqual((long long)100, integer);
	Assert::IsTrue(expected == str_parser.tell());
}
void take_signed_integer_test::test_minus() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "-100";
	std::streamoff expected = stream.tellp();

	stream_parser str_parser(std::move(stream));
	auto          integer = str_parser.take_signed_integer<long long>();
	Assert::AreEqual((long long)-100, integer);
	Assert::IsTrue(expected == str_parser.tell());
}
void take_signed_integer_test::test_max() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	constexpr auto max = std::numeric_limits<long long>::max();
	stream << max;
	std::streamoff expected = stream.tellp();

	stream_parser str_parser(std::move(stream));
	auto          integer = str_parser.take_signed_integer<long long>();
	Assert::AreEqual(max, integer);
	Assert::IsTrue(expected == str_parser.tell());
}
void take_signed_integer_test::test_min() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	constexpr auto min = std::numeric_limits<long long>::min();
	stream << min;
	std::streamoff expected = stream.tellp();

	stream_parser str_parser(std::move(stream));
	auto          integer = str_parser.take_signed_integer<long long>();
	Assert::AreEqual(min, integer);
	Assert::IsTrue(expected == str_parser.tell());
}

void take_signed_integer_test::test_not_number() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "ABC";

	stream_parser str_parser(std::move(stream));
	try {
		str_parser.take_signed_integer<long long>();
	} catch (const parse_error& parse_e) {
		Assert::IsTrue(parse_error::signed_integer_not_found == parse_e.code());

		// success
		return;
	}
	Assert::Fail();
}
void take_signed_integer_test::test_EOF() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream_parser str_parser(std::move(stream));
	try {
		str_parser.take_signed_integer<long long>();
	} catch (const parse_error& parse_e) {
		Assert::IsTrue(parse_error::signed_integer_not_found == parse_e.code());

		// success
		return;
	}
	Assert::Fail();
}
void take_signed_integer_test::test_plus_not_number() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "+deadbeef";

	stream_parser str_parser(std::move(stream));
	try {
		str_parser.take_signed_integer<long long>();
	} catch (const parse_error& parse_e) {
		Assert::IsTrue(parse_error::signed_integer_not_found == parse_e.code());

		// success
		return;
	}
	Assert::Fail();
}
void take_signed_integer_test::test_minus_not_number() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "-deadbeef";

	stream_parser str_parser(std::move(stream));
	try {
		str_parser.take_signed_integer<long long>();
	} catch (const parse_error& parse_e) {
		Assert::IsTrue(parse_error::signed_integer_not_found == parse_e.code());

		// success
		return;
	}
	Assert::Fail();
}
void take_signed_integer_test::test_sign_EOF() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "+";

	stream_parser str_parser(std::move(stream));
	try {
		str_parser.take_signed_integer<long long>();
	} catch (const parse_error& parse_e) {
		Assert::IsTrue(parse_error::signed_integer_not_found == parse_e.code());

		// success
		return;
	}
	Assert::Fail();
}
void take_signed_integer_test::test_max_plus1() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	constexpr auto max = std::numeric_limits<long long>::max();
	stream << max / 10 << ((max % 10) + 1);

	stream_parser str_parser(std::move(stream));
	try {
		str_parser.take_signed_integer<long long>();
	} catch (std::overflow_error&) {
		// success
		return;
	}
	Assert::Fail();
}
void take_signed_integer_test::test_min_minus1() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	constexpr auto min = std::numeric_limits<long long>::min();
	stream << min / 10 << (-(min % 10) + 1);

	stream_parser str_parser(std::move(stream));
	try {
		str_parser.take_signed_integer<long long>();
	} catch (std::overflow_error&) {
		// success
		return;
	}
	Assert::Fail();
}
