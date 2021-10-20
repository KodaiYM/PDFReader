#include "literal_trim.hpp"
#include "pdfparser.object_cache.hpp"
#include "pdfparser.object_stream.hpp"
#include "pdfparser.object_stream_errors.hpp"
#include "take_object_test.hpp"

#include <sstream>

using namespace pdfparser;
using namespace object_types;
using namespace object_stream_test;

void take_object_test::test_any_direct_object_boolean() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "true";

	object_stream           obj_stream(stream.rdbuf());
	onstream_boolean_object object = obj_stream.take_object();
	Assert::IsTrue(true == object);
	Assert::IsTrue(0 == object.position());
}
void take_object_test::test_any_direct_object_integer() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "10";

	object_stream           obj_stream(stream.rdbuf());
	onstream_integer_object object = obj_stream.take_object();
	Assert::IsTrue(integer_object{10} == object);
	Assert::IsTrue(0 == object.position());
}
void take_object_test::test_any_direct_object_real() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "1.2";

	object_stream        obj_stream(stream.rdbuf());
	onstream_real_object object = obj_stream.take_object();
	Assert::IsTrue(1.2 == object);
	Assert::IsTrue(0 == object.position());
}
void take_object_test::test_any_direct_object_string() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "(str)";

	object_stream          obj_stream(stream.rdbuf());
	onstream_string_object object = obj_stream.take_object();
	Assert::IsTrue("str" == object);
	Assert::IsTrue(0 == object.position());
}
void take_object_test::test_any_direct_object_name() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "/name";

	object_stream        obj_stream(stream.rdbuf());
	onstream_name_object object = obj_stream.take_object();
	Assert::IsTrue("name" == object);
	Assert::IsTrue(0 == object.position());
}
void take_object_test::test_any_direct_object_array() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "[(s) 1 0.4]";

	object_stream         obj_stream(stream.rdbuf());
	onstream_array_object object = obj_stream.take_object();
	Assert::IsTrue(array_object{string_object("s"), 1, 0.4} == object);
	Assert::IsTrue(0 == object.position());
}
void take_object_test::test_any_direct_object_dictionary() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "<</key(value)>>";

	object_stream              obj_stream(stream.rdbuf());
	onstream_dictionary_object object = obj_stream.take_object();
	Assert::IsTrue(dictionary_object{{"key", string_object("value")}} == object);
	Assert::IsTrue(0 == object.position());
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

	object_stream          obj_stream(stream.rdbuf());
	onstream_stream_object object = obj_stream.take_object();
	Assert::IsTrue(stream_object{dictionary_object{{"Length", 9}}, "123456789"} ==
	               object);
	Assert::IsTrue(0 == object.position());
}
void take_object_test::test_any_direct_object_null() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "null";

	object_stream        obj_stream(stream.rdbuf());
	onstream_null_object object = obj_stream.take_object();
	Assert::IsTrue(null_object{} == object);
	Assert::IsTrue(0 == object.position());
}
void take_object_test::test_any_direct_object_indirect_reference() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "1 2 R";

	object_stream               obj_stream(stream.rdbuf());
	onstream_indirect_reference object = obj_stream.take_object();
	Assert::IsTrue(indirect_reference{1, 2} == object);
	Assert::IsTrue(0 == object.position());
}

void take_object_test::test_no_object() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "nothing";

	object_stream obj_stream(stream.rdbuf());
	try {
		obj_stream.take_object();
	} catch (specified_object_not_found&) {
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
		obj_stream.take_object();
	} catch (specified_object_not_found&) {
		// success
		return;
	}
	Assert::Fail();
}
