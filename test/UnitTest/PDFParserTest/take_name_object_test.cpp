#include "testtool.h"

namespace object_stream_test {
[TestClass] public ref class take_name_object_test {
public:
	[TestMethod] void test_valid_name();
	[TestMethod] void test_empty_name();
	[TestMethod] void test_hexadecimal_code();
};
} // namespace object_stream_test

#include "pdfparser.object_stream.hpp"

#include <sstream>
#include <string>

using namespace pdfparser;
using namespace object_types;
using namespace object_stream_test;

using namespace std::string_literals;

void take_name_object_test::test_valid_name() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << R"(/;*_-.$@!"&'=~^\|`+:,?)";

	object_stream obj_stream(stream.rdbuf());

	auto object = obj_stream.take_name_object();
	Assert::IsTrue(R"(;*_-.$@!"&'=~^\|`+:,?)" == object);
	Assert::IsTrue(0 == object.position());
}
void take_name_object_test::test_empty_name() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "/";

	object_stream obj_stream(stream.rdbuf());

	auto object = obj_stream.take_name_object();
	Assert::IsTrue("" == object);
	Assert::IsTrue(0 == object.position());
}
void take_name_object_test::test_hexadecimal_code() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "/A#20B";

	object_stream obj_stream(stream.rdbuf());

	auto object = obj_stream.take_name_object();
	Assert::IsTrue("A"s + '\x20' + "B" == object);
	Assert::IsTrue(0 == object.position());
}
