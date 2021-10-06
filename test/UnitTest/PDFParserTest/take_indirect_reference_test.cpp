#include "pdfparser.object_stream.hpp"
#include "take_indirect_reference_test.hpp"

#include <sstream>

using namespace pdfparser;

using namespace object_types;
using namespace object_stream_test;

void take_indirect_reference_test::test_unsigned() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "3 8 R";

	object_stream obj_stream(stream.rdbuf());

	auto object = obj_stream.take_indirect_reference();
	Assert::IsTrue(indirect_reference{3, 8} == object);
}
void take_indirect_reference_test::test_signed() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "+3 +8 R";

	object_stream obj_stream(stream.rdbuf());

	auto object = obj_stream.take_indirect_reference();
	Assert::IsTrue(indirect_reference{3, 8} == object);
}
