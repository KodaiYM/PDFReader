#include "PDFParser.h"
#include "take_any_object_test@integer_object_test.hpp"

#include <sstream>

using namespace pdfparser;
using namespace error_types;
using namespace object_types;
using namespace pdfparser_test;

using namespace take_any_object_test;

void integer_object_test::test_unsigned_integer() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "0123";

	stream_parser str_parser(std::move(stream));
	object_pool   obj_pool(str_parser);
	auto          object = str_parser.take_any_object(obj_pool);
	Assert::IsTrue(123 == std::get<integer_object>(object));
}
void integer_object_test::test_plus_integer() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "+0123";

	stream_parser str_parser(std::move(stream));
	object_pool   obj_pool(str_parser);
	auto          object = str_parser.take_any_object(obj_pool);
	Assert::IsTrue(123 == std::get<integer_object>(object));
}
void integer_object_test::test_minus_integer() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "-0123";

	stream_parser str_parser(std::move(stream));
	object_pool   obj_pool(str_parser);
	auto          object = str_parser.take_any_object(obj_pool);
	Assert::IsTrue(-123 == std::get<integer_object>(object));
}
