#include "pdfparser.document_parser.hpp"
#include "take_indirect_reference_test.hpp"

#include <sstream>

using namespace pdfparser;

using namespace object_types;
using namespace stream_parser_test::take_object_test;

void take_indirect_reference_test::test_unsigned() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "3 8 R";

	document_parser str_parser(std::move(stream));

	auto object = str_parser.take_indirect_reference();
	Assert::IsTrue(indirect_reference{3, 8} == object);
}
void take_indirect_reference_test::test_signed() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "+3 +8 R";

	document_parser str_parser(std::move(stream));

	auto object = str_parser.take_indirect_reference();
	Assert::IsTrue(indirect_reference{3, 8} == object);
}
