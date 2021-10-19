#include "pdfparser.object_stream.hpp"
#include "take_boolean_object_test.hpp"

#include <sstream>

using namespace pdfparser;
using namespace object_types;
using namespace object_stream_test;

void take_boolean_object_test::test_true() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "true";

	object_stream obj_stream(stream.rdbuf());

	auto object = obj_stream.take_boolean_object();
	Assert::IsTrue(true == object);
	Assert::IsTrue(0 == object.position());
}
void take_boolean_object_test::test_false() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "false";

	object_stream obj_stream(stream.rdbuf());

	auto object = obj_stream.take_boolean_object();
	Assert::IsTrue(false == object);
	Assert::IsTrue(0 == object.position());
}
