#include "testtool.h"

namespace object_stream_test {
[TestClass] public ref class take_null_object_test {
public:
	[TestMethod] void test_null();
};
} // namespace object_stream_test

#include "pdfparser.object_stream.hpp"

#include <sstream>

using namespace pdfparser;
using namespace object_types;
using namespace object_stream_test;

void take_null_object_test::test_null() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "null";

	object_stream obj_stream(stream.rdbuf());
	auto          object = obj_stream.take_null_object();
	Assert::IsTrue(null_object{} == object);
	Assert::IsTrue(0 == object.position());
}
