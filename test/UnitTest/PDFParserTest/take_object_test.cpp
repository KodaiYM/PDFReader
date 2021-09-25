#include "literal_trim.hpp"
#include "pdfparser.object_pool.hpp"
#include "pdfparser.stream_parser.hpp"
#include "take_object_test.hpp"

#include <sstream>

using namespace pdfparser;
using namespace object_types;
using namespace stream_parser_test::take_object_test;

void take_object_test::test_any_direct_object_boolean() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "true";

	document_parser str_parser(std::move(stream));
	object_pool     obj_pool(str_parser);
	auto object = str_parser.take_object<any_direct_object_or_ref>(obj_pool);
	Assert::IsTrue(true == std::get<boolean_object>(object));
}
void take_object_test::test_any_direct_object_integer() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "10";

	document_parser str_parser(std::move(stream));
	object_pool     obj_pool(str_parser);
	auto object = str_parser.take_object<any_direct_object_or_ref>(obj_pool);
	Assert::IsTrue(10 == static_cast<int>(std::get<integer_object>(object)));
}
void take_object_test::test_any_direct_object_real() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "1.2";

	document_parser str_parser(std::move(stream));
	object_pool     obj_pool(str_parser);
	auto object = str_parser.take_object<any_direct_object_or_ref>(obj_pool);
	Assert::IsTrue(1.2 == std::get<real_object>(object));
}
void take_object_test::test_any_direct_object_string() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "(str)";

	document_parser str_parser(std::move(stream));
	object_pool     obj_pool(str_parser);
	auto object = str_parser.take_object<any_direct_object_or_ref>(obj_pool);
	Assert::IsTrue("str" == std::get<string_object>(object));
}
void take_object_test::test_any_direct_object_name() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "/name";

	document_parser str_parser(std::move(stream));
	object_pool     obj_pool(str_parser);
	auto object = str_parser.take_object<any_direct_object_or_ref>(obj_pool);
	Assert::IsTrue("name" == std::get<name_object>(object));
}
void take_object_test::test_any_direct_object_array() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "[(s) 1 0.4]";

	document_parser str_parser(std::move(stream));
	object_pool     obj_pool(str_parser);
	auto object = str_parser.take_object<any_direct_object_or_ref>(obj_pool);
	Assert::IsTrue(array_object{string_object("s"), 1, 0.4} ==
	               std::get<array_object>(object));
}
void take_object_test::test_any_direct_object_dictionary() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "<</key(value)>>";

	document_parser str_parser(std::move(stream));
	object_pool     obj_pool(str_parser);
	auto object = str_parser.take_object<any_direct_object_or_ref>(obj_pool);
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

	document_parser str_parser(std::move(stream));
	object_pool     obj_pool(str_parser);
	auto object = str_parser.take_object<any_direct_object_or_ref>(obj_pool);
	Assert::IsTrue(stream_object{dictionary_object{{"Length", 9}}, "123456789"} ==
	               std::get<stream_object>(object));
}
void take_object_test::test_any_direct_object_null() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "null";

	document_parser str_parser(std::move(stream));
	object_pool     obj_pool(str_parser);
	auto object = str_parser.take_object<any_direct_object_or_ref>(obj_pool);
	Assert::IsTrue(null == std::get<null_object>(object));
}
void take_object_test::test_any_direct_object_indirect_reference() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "1 2 R";

	document_parser str_parser(std::move(stream));
	object_pool     obj_pool(str_parser);
	auto object = str_parser.take_object<any_direct_object_or_ref>(obj_pool);
	Assert::IsTrue(indirect_reference{1, 2} ==
	               std::get<indirect_reference>(object));
}

void take_object_test::test_no_object() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "nothing";

	document_parser str_parser(std::move(stream));
	object_pool     obj_pool(str_parser);
	try {
		str_parser.take_object<any_direct_object>(obj_pool);
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

	document_parser str_parser(std::move(stream));
	object_pool     obj_pool(str_parser);
	try {
		str_parser.take_object<any_direct_object>(obj_pool);
	} catch (const object_not_found_error& obj_e) {
		Assert::IsTrue(object_not_found_error::specified_object_not_found ==
		               obj_e.code());

		// success
		return;
	}
	Assert::Fail();
}
