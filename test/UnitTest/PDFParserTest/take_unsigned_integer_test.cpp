#include "PDFParser.h"
#include "take_unsigned_integer_test.hpp"

#include <sstream>

using namespace pdfparser;
using namespace error_types;
using namespace object_types;
using namespace pdfparser_test;

void take_unsigned_integer_test::test_unsigned() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "17";
	std::streamoff expected = stream.tellp();

	stream_parser str_parser(std::move(stream));
	auto          integer = str_parser.take_unsigned_integer<long long>();
	Assert::AreEqual((long long)17, integer);
	Assert::IsTrue(expected == str_parser.tell());
}
void take_unsigned_integer_test::test_max() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	constexpr auto max = std::numeric_limits<unsigned long long>::max();
	stream << max;
	std::streamoff expected = stream.tellp();

	stream_parser str_parser(std::move(stream));
	auto integer = str_parser.take_unsigned_integer<unsigned long long>();
	Assert::AreEqual(max, integer);
	Assert::IsTrue(expected == str_parser.tell());
}
void take_unsigned_integer_test::test_min() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << 0;
	std::streamoff expected = stream.tellp();

	stream_parser str_parser(std::move(stream));
	auto integer = str_parser.take_unsigned_integer<unsigned long long>();
	Assert::AreEqual((unsigned long long)0, integer);
	Assert::IsTrue(expected == str_parser.tell());
}

void take_unsigned_integer_test::test_not_number() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "deadbeef";

	stream_parser str_parser(std::move(stream));
	try {
		str_parser.take_unsigned_integer<long long>();
	} catch (const parse_error& parse_e) {
		Assert::IsTrue(parse_error::unsigned_integer_not_found == parse_e.code());

		// success
		return;
	}
	Assert::Fail();
}
void take_unsigned_integer_test::test_EOF() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream_parser str_parser(std::move(stream));
	try {
		str_parser.take_unsigned_integer<long long>();
	} catch (const parse_error& parse_e) {
		Assert::IsTrue(parse_error::unsigned_integer_not_found == parse_e.code());

		// success
		return;
	}
	Assert::Fail();
}
void take_unsigned_integer_test::test_signed() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "+17";

	stream_parser str_parser(std::move(stream));
	try {
		str_parser.take_unsigned_integer<long long>();
	} catch (const parse_error& parse_e) {
		Assert::IsTrue(parse_error::unsigned_integer_not_found == parse_e.code());

		// success
		return;
	}
	Assert::Fail();
}
void take_unsigned_integer_test::test_max_plus1() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	constexpr auto max = std::numeric_limits<unsigned long long>::max();
	stream << (max / 10) << (max % 10 + 1);

	stream_parser str_parser(std::move(stream));
	try {
		str_parser.take_unsigned_integer<long long>();
	} catch (std::overflow_error&) {
		// success
		return;
	}
	Assert::Fail();
}
