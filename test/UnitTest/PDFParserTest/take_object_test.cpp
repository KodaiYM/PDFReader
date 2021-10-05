#include "literal_trim.hpp"
#include "pdfparser.ipdfstream.hpp"
#include "pdfparser.object_pool.hpp"
#include "take_object_test.hpp"

#include <sstream>

using namespace pdfparser;
using namespace object_types;
using namespace ipdfstream_test::take_object_test;

void take_object_test::test_any_direct_object_boolean() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "true";

	ipdfstream str_parser(stream.rdbuf());
	auto       object = str_parser.take_object<any_direct_object_or_ref>();
	Assert::IsTrue(true == std::get<boolean_object>(object));
}
void take_object_test::test_any_direct_object_integer() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "10";

	ipdfstream str_parser(stream.rdbuf());
	auto       object = str_parser.take_object<any_direct_object_or_ref>();
	Assert::IsTrue(10 == static_cast<int>(std::get<integer_object>(object)));
}
void take_object_test::test_any_direct_object_real() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "1.2";

	ipdfstream str_parser(stream.rdbuf());
	auto       object = str_parser.take_object<any_direct_object_or_ref>();
	Assert::IsTrue(1.2 == std::get<real_object>(object));
}
void take_object_test::test_any_direct_object_string() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "(str)";

	ipdfstream str_parser(stream.rdbuf());
	auto       object = str_parser.take_object<any_direct_object_or_ref>();
	Assert::IsTrue("str" == std::get<string_object>(object));
}
void take_object_test::test_any_direct_object_name() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "/name";

	ipdfstream str_parser(stream.rdbuf());
	auto       object = str_parser.take_object<any_direct_object_or_ref>();
	Assert::IsTrue("name" == std::get<name_object>(object));
}
void take_object_test::test_any_direct_object_array() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "[(s) 1 0.4]";

	ipdfstream str_parser(stream.rdbuf());
	auto       object = str_parser.take_object<any_direct_object_or_ref>();
	Assert::IsTrue(array_object{string_object("s"), 1, 0.4} ==
	               std::get<array_object>(object));
}
void take_object_test::test_any_direct_object_dictionary() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "<</key(value)>>";

	ipdfstream str_parser(stream.rdbuf());
	auto       object = str_parser.take_object<any_direct_object_or_ref>();
	Assert::IsTrue(dictionary_object{{"key", string_object("value")}} ==
	               std::get<dictionary_object>(object));
}
void take_object_test::test_any_direct_object_stream() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << R"(
<</Length 9>>
stream
123456789
endstream
)"_trimmed;

	ipdfstream str_parser(stream.rdbuf());
	auto       object = str_parser.take_object<any_direct_object_or_ref>();
	Assert::IsTrue(stream_object{dictionary_object{{"Length", 9}}, "123456789"} ==
	               std::get<stream_object>(object));
}
void take_object_test::test_any_direct_object_null() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "null";

	ipdfstream str_parser(stream.rdbuf());
	auto       object = str_parser.take_object<any_direct_object_or_ref>();
	Assert::IsTrue(null == std::get<null_object>(object));
}
void take_object_test::test_any_direct_object_indirect_reference() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "1 2 R";

	ipdfstream str_parser(stream.rdbuf());
	auto       object = str_parser.take_object<any_direct_object_or_ref>();
	Assert::IsTrue(indirect_reference{1, 2} ==
	               std::get<indirect_reference>(object));
}

void take_object_test::test_no_object() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "nothing";

	ipdfstream str_parser(stream.rdbuf());
	try {
		str_parser.take_object<any_direct_object>();
	} catch (const object_not_found_error& obj_e) {
		Assert::IsTrue(object_not_found_error::specified_object_not_found ==
		               obj_e.code());

		// success
		return;
	}
	Assert::Fail();
}
void take_object_test::test_eof() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	ipdfstream str_parser(stream.rdbuf());
	try {
		str_parser.take_object<any_direct_object>();
	} catch (const object_not_found_error& obj_e) {
		Assert::IsTrue(object_not_found_error::specified_object_not_found ==
		               obj_e.code());

		// success
		return;
	}
	Assert::Fail();
}
