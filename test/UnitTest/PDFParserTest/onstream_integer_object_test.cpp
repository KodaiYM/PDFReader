#include "testtool.h"

namespace object_types_test {
[TestClass] public ref class onstream_integer_object_test {
public:
	[TestMethod] void test_overflow_when_construction();
	[TestMethod] void test_overflow_when_conversion();

	[TestMethod] void test_overflow_when_construct_from_string();
};
} // namespace object_types_test

#include "AssertThrows.hpp"
#include "pdfparser.object_types.hpp"

#include <cstdint>
#include <limits>

using namespace pdfparser;
using namespace pdfparser::object_types;

using namespace object_types_test;

void onstream_integer_object_test::test_overflow_when_construction() {
	using int_type                = integer_object::int_type;
	using uint_type               = std::make_unsigned_t<int_type>;
	constexpr int_type  max       = std::numeric_limits<int_type>::max();
	constexpr uint_type max_plus1 = static_cast<uint_type>(max) + 1;

	AssertThrows(onstream_integer_object_overflows,
	             onstream_integer_object{0, max_plus1});
}

void onstream_integer_object_test::test_overflow_when_conversion() {
	using smaller_type = int_least8_t;
	static_assert(sizeof(smaller_type) < sizeof(integer_object::int_type),
	              "These tests must be run with a type smaller than "
	              "onstream_integer_object::int_type.");
	constexpr auto max = std::numeric_limits<integer_object::int_type>::max();

	AssertThrows(onstream_integer_object_overflows,
	             static_cast<smaller_type>(onstream_integer_object{0, max}));
}

void onstream_integer_object_test::test_overflow_when_construct_from_string() {
	using int_type                = integer_object::int_type;
	using uint_type               = std::make_unsigned_t<int_type>;
	constexpr int_type  max       = std::numeric_limits<int_type>::max();
	constexpr uint_type max_plus1 = static_cast<uint_type>(max) + 1;

	AssertThrows(onstream_integer_object_overflows,
	             onstream_integer_object{0, std::to_string(max_plus1)});
}
