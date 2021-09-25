#include "pdfparser.document_parser.hpp"
#include "take_null_object_test.hpp"

#include <sstream>

using namespace pdfparser;
using namespace object_types;
using namespace document_parser_test::take_object_test;

void take_null_object_test::test_null() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "null";

	document_parser str_parser(std::move(stream));
	auto            object = str_parser.take_null_object();
	Assert::IsTrue(null == object);
}
