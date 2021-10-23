#include "testtool.h"

namespace object_stream_test {
[TestClass] public ref class take_literal_string_test {
public:
	[TestMethod] void test_sample();
	[TestMethod] void test_escape_sequence();
	[TestMethod] void test_invalid_escape_sequence();
	[TestMethod] void test_escaped_EOL();
	[TestMethod] void test_EOL_unification();
	[TestMethod] void test_short_octal();
	[TestMethod] void test_octal_overflow();
	[TestMethod] void test_lack_of_right_parenthesis();
};
} // namespace object_stream_test

#include "AssertThrows.hpp"
#include "literal_trim.hpp"
#include "pdfparser.object_stream.hpp"
#include "pdfparser.object_stream_errors.hpp"

#include <sstream>

using namespace pdfparser;
using namespace object_types;
using namespace object_stream_test;

void take_literal_string_test::test_sample() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "( String may contain balanced parentheses () and\n"
	          "special characters (*!&}^% and so on). )";

	object_stream obj_stream(stream.rdbuf());

	auto object = obj_stream.take_string_object();
	Assert::IsTrue(" String may contain balanced parentheses () and\nspecial"
	               " characters (*!&}^% and so on). " == object);
	Assert::IsTrue(0 == object.position());
}
void take_literal_string_test::test_escape_sequence() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << R"((\n\r\r\n\t\b\f\(\)\\\123))";

	object_stream obj_stream(stream.rdbuf());

	auto object = obj_stream.take_string_object();
	Assert::IsTrue("\n\r\r\n\t\b\f()\\\123" == object);
	Assert::IsTrue(0 == object.position());
}
void take_literal_string_test::test_invalid_escape_sequence() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "(\\a)";

	object_stream obj_stream(stream.rdbuf());

	auto object = obj_stream.take_string_object();
	Assert::IsTrue("a" == object);
	Assert::IsTrue(0 == object.position());
}
void take_literal_string_test::test_escaped_EOL() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << R"(
(These \
two strings \
are the same.)
)"_trimmed;

	object_stream obj_stream(stream.rdbuf());

	auto object = obj_stream.take_string_object();
	Assert::IsTrue("These two strings are the same." == object);
	Assert::IsTrue(0 == object.position());
}
void take_literal_string_test::test_EOL_unification() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "(\n\r\n\r)";

	object_stream obj_stream(stream.rdbuf());

	auto object = obj_stream.take_string_object();
	Assert::IsTrue("\n\n\n" == object);
	Assert::IsTrue(0 == object.position());
}
void take_literal_string_test::test_short_octal() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "(before\\56after)";

	object_stream obj_stream(stream.rdbuf());

	auto object = obj_stream.take_string_object();
	Assert::IsTrue("before\56after" == object);
	Assert::IsTrue(0 == object.position());
}
void take_literal_string_test::test_octal_overflow() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "(\\777)";

	object_stream obj_stream(stream.rdbuf());

	auto object = obj_stream.take_string_object();
	Assert::IsTrue("\377" == object);
	Assert::IsTrue(0 == object.position());
}
void take_literal_string_test::test_lack_of_right_parenthesis() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "(abc(\\)def)";

	object_stream obj_stream(stream.rdbuf());

	AssertThrows(literal_string_lack_of_right_parenthesis,
	             obj_stream.take_string_object());
}
