#include "pdfparser.stream_parser.hpp"
#include "take_indirect_reference_test.hpp"

#include <sstream>

using namespace pdfparser;

using namespace object_types;
using namespace pdfparser_test;

void take_indirect_reference_test::test_unsigned() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "3 8 R";

	stream_parser str_parser(std::move(stream));

	auto object = str_parser.take_indirect_reference();
	Assert::IsTrue(indirect_reference{3, 8} == object);
}
void take_indirect_reference_test::test_signed() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "+3 +8 R";

	stream_parser str_parser(std::move(stream));

	auto object = str_parser.take_indirect_reference();
	Assert::IsTrue(indirect_reference{3, 8} == object);
}
