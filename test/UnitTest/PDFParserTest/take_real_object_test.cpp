#include "pdfparser.document_parser.hpp"
#include "take_real_object_test.hpp"

#include <sstream>

using namespace pdfparser;
using namespace object_types;
using namespace document_parser_test::take_object_test;

void take_real_object_test::test_unsigned_leading_period() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << ".0123";

	document_parser str_parser(std::move(stream));

	auto object = str_parser.take_real_object();
	Assert::IsTrue(0.0123 == object);
}
void take_real_object_test::test_unsigned_trailing_period() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "0123.";

	document_parser str_parser(std::move(stream));

	auto object = str_parser.take_real_object();
	Assert::IsTrue(123.0 == object);
}
void take_real_object_test::test_unsigned_embedded_period() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "34.5";

	document_parser str_parser(std::move(stream));

	auto object = str_parser.take_real_object();
	Assert::IsTrue(34.5 == object);
}
void take_real_object_test::test_plus_leading_period() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "+.002";

	document_parser str_parser(std::move(stream));

	auto object = str_parser.take_real_object();
	Assert::IsTrue(0.002 == object);
}
void take_real_object_test::test_plus_trailing_period() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "+0123.";

	document_parser str_parser(std::move(stream));

	auto object = str_parser.take_real_object();
	Assert::IsTrue(123.0 == object);
}
void take_real_object_test::test_plus_embedded_period() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "+0123.6";

	document_parser str_parser(std::move(stream));

	auto object = str_parser.take_real_object();
	Assert::IsTrue(123.6 == object);
}
void take_real_object_test::test_minus_leading_period() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "-.002";

	document_parser str_parser(std::move(stream));

	auto object = str_parser.take_real_object();
	Assert::IsTrue(-0.002 == object);
}
void take_real_object_test::test_minus_trailing_period() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "-04.";

	document_parser str_parser(std::move(stream));

	auto object = str_parser.take_real_object();
	Assert::IsTrue(-4.0 == object);
}
void take_real_object_test::test_minus_embedded_period() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "-3.62";

	document_parser str_parser(std::move(stream));

	auto object = str_parser.take_real_object();
	Assert::IsTrue(-3.62 == object);
}
