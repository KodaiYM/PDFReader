#include "literal_trim.hpp"
#include "pdfparser.object_cache.hpp"
#include "pdfparser.object_not_found_error.hpp"
#include "pdfparser.object_stream.hpp"
#include "take_object_test.hpp"

#include <sstream>

using namespace pdfparser;
using namespace object_types;
using namespace object_stream_test;

void take_object_test::test_any_direct_object_boolean() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "true";

	object_stream obj_stream(stream.rdbuf());
	auto          object = obj_stream.take_object<any_direct_object_or_ref>();
	Assert::IsTrue(true == static_cast<boolean_object>(object));
}
void take_object_test::test_any_direct_object_integer() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "10";

	object_stream obj_stream(stream.rdbuf());
	auto          object = obj_stream.take_object<any_direct_object_or_ref>();
	Assert::IsTrue(10 == static_cast<int>(static_cast<integer_object>(object)));
}
void take_object_test::test_any_direct_object_real() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "1.2";

	object_stream obj_stream(stream.rdbuf());
	auto          object = obj_stream.take_object<any_direct_object_or_ref>();
	Assert::IsTrue(1.2 == static_cast<real_object>(object));
}
void take_object_test::test_any_direct_object_string() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "(str)";

	object_stream obj_stream(stream.rdbuf());
	auto          object = obj_stream.take_object<any_direct_object_or_ref>();
	Assert::IsTrue("str" == static_cast<string_object>(object));
}
void take_object_test::test_any_direct_object_name() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "/name";

	object_stream obj_stream(stream.rdbuf());
	auto          object = obj_stream.take_object<any_direct_object_or_ref>();
	Assert::IsTrue("name" == static_cast<name_object>(object));
}
void take_object_test::test_any_direct_object_array() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "[(s) 1 0.4]";

	object_stream obj_stream(stream.rdbuf());
	auto          object = obj_stream.take_object<any_direct_object_or_ref>();
	Assert::IsTrue(array_object{string_object("s"), 1, 0.4} ==
	               static_cast<array_object>(object));
}
void take_object_test::test_any_direct_object_dictionary() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "<</key(value)>>";

	object_stream obj_stream(stream.rdbuf());
	auto          object = obj_stream.take_object<any_direct_object_or_ref>();
	Assert::IsTrue(dictionary_object{{"key", string_object("value")}} ==
	               static_cast<dictionary_object>(object));
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

	object_stream obj_stream(stream.rdbuf());
	auto          object = obj_stream.take_object<any_direct_object_or_ref>();
	Assert::IsTrue(stream_object{dictionary_object{{"Length", 9}}, "123456789"} ==
	               static_cast<stream_object>(object));
}
void take_object_test::test_any_direct_object_null() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "null";

	object_stream obj_stream(stream.rdbuf());
	auto          object = obj_stream.take_object<any_direct_object_or_ref>();
	Assert::IsTrue(null_object{} == static_cast<null_object>(object));
}
void take_object_test::test_any_direct_object_indirect_reference() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "1 2 R";

	object_stream obj_stream(stream.rdbuf());
	auto          object = obj_stream.take_object<any_direct_object_or_ref>();
	Assert::IsTrue(indirect_reference{1, 2} ==
	               static_cast<indirect_reference>(object));
}

void take_object_test::test_no_object() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "nothing";

	object_stream obj_stream(stream.rdbuf());
	try {
		obj_stream.take_object<any_direct_object>();
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

	object_stream obj_stream(stream.rdbuf());
	try {
		obj_stream.take_object<any_direct_object>();
	} catch (const object_not_found_error& obj_e) {
		Assert::IsTrue(object_not_found_error::specified_object_not_found ==
		               obj_e.code());

		// success
		return;
	}
	Assert::Fail();
}
