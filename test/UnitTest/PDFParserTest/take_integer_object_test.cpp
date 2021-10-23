#include "testtool.h"

namespace object_stream_test {
[TestClass] public ref class take_integer_object_test {
public:
	[TestMethod] void test_unsigned_integer();
	[TestMethod] void test_plus_integer();
	[TestMethod] void test_minus_integer();
	[TestMethod] void test_max();
	[TestMethod] void test_min();

	[TestMethod] void test_not_number();
	[TestMethod] void test_EOF();
	[TestMethod] void test_plus_not_number();
	[TestMethod] void test_minus_not_number();
	[TestMethod] void test_sign_EOF();
	[TestMethod] void test_max_plus1();
	[TestMethod] void test_min_minus1();
};
} // namespace object_stream_test

#include "AssertThrows.hpp"
#include "pdfparser.object_stream.hpp"
#include "pdfparser.object_stream_errors.hpp"

#include <sstream>

using namespace pdfparser;
using namespace object_types;
using namespace object_stream_test;

void take_integer_object_test::test_unsigned_integer() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "0123";

	object_stream obj_stream(stream.rdbuf());

	auto object = obj_stream.take_integer_object();
	Assert::AreEqual(123, static_cast<int>(object));
	Assert::IsTrue(0 == object.position());
}
void take_integer_object_test::test_plus_integer() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "+0123";

	object_stream obj_stream(stream.rdbuf());

	auto object = obj_stream.take_integer_object();
	Assert::AreEqual(123, static_cast<int>(object));
	Assert::IsTrue(0 == object.position());
}
void take_integer_object_test::test_minus_integer() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "-0123";

	object_stream obj_stream(stream.rdbuf());

	auto object = obj_stream.take_integer_object();
	Assert::AreEqual(-123, static_cast<int>(object));
	Assert::IsTrue(0 == object.position());
}
void take_integer_object_test::test_max() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	constexpr integer_object::int_type max =
	    std::numeric_limits<decltype(max)>::max();
	stream << max;
	std::streamoff expected = stream.tellp();

	object_stream            obj_stream(stream.rdbuf());
	integer_object::int_type integer = obj_stream.take_integer_object();
	Assert::AreEqual(max, integer);
}
void take_integer_object_test::test_min() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	constexpr integer_object::int_type min =
	    std::numeric_limits<decltype(min)>::min();
	stream << min;
	std::streamoff expected = stream.tellp();

	object_stream            obj_stream(stream.rdbuf());
	integer_object::int_type integer = obj_stream.take_integer_object();
	Assert::AreEqual(min, integer);
}

void take_integer_object_test::test_not_number() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "deadbeef";

	object_stream obj_stream(stream.rdbuf());

	AssertThrows(integer_object_not_found, obj_stream.take_integer_object());
}
void take_integer_object_test::test_EOF() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	object_stream obj_stream(stream.rdbuf());

	AssertThrows(integer_object_not_found, obj_stream.take_integer_object());
}
void take_integer_object_test::test_plus_not_number() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "+deadbeef";

	object_stream obj_stream(stream.rdbuf());

	AssertThrows(integer_object_not_found, obj_stream.take_integer_object());
}
void take_integer_object_test::test_minus_not_number() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "-deadbeef";

	object_stream obj_stream(stream.rdbuf());

	AssertThrows(integer_object_not_found, obj_stream.take_integer_object());
}
void take_integer_object_test::test_sign_EOF() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "+";

	object_stream obj_stream(stream.rdbuf());

	AssertThrows(integer_object_not_found, obj_stream.take_integer_object());
}
void take_integer_object_test::test_max_plus1() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	constexpr auto max = std::numeric_limits<integer_object::int_type>::max();
	stream << max / 10 << ((max % 10) + 1);

	object_stream obj_stream(stream.rdbuf());

	AssertThrows(onstream_integer_object_overflows,
	             obj_stream.take_integer_object());
}
void take_integer_object_test::test_min_minus1() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	constexpr auto min = std::numeric_limits<long long>::min();
	stream << min / 10 << (-(min % 10) + 1);

	object_stream obj_stream(stream.rdbuf());

	AssertThrows(onstream_integer_object_overflows,
	             obj_stream.take_integer_object());
}
