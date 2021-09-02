#include "pdfparser.stream_parser.hpp"
#include "take_null_object_test.hpp"

#include <sstream>

using namespace pdfparser;
using namespace object_types;
using namespace pdfparser_test;

void take_null_object_test::test_null() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "null";

	stream_parser str_parser(std::move(stream));
	auto          object = str_parser.take_null_object();
	Assert::IsTrue(null == object);
}
