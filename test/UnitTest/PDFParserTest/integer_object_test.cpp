#include "testtool.h"

namespace object_types_test {
[TestClass] public ref class integer_object_test {
public:
	[TestMethod] void test_min_when_construction();
	[TestMethod] void test_max_when_construction();
	[TestMethod] void test_overflow_max_when_construction();

	[TestMethod] void test_min_when_conversion();
	[TestMethod] void test_max_when_conversion();
	[TestMethod] void test_overflow_min_when_conversion();
	[TestMethod] void test_overflow_max_when_conversion();
};
} // namespace object_types_test

#include "AssertThrows.hpp"
#include "pdfparser.object_types.hpp"

#include <cstdint>
#include <limits>

using namespace pdfparser;
using namespace pdfparser::object_types;

using namespace object_types_test;

void integer_object_test::test_min_when_construction() {
	using int_type     = integer_object::int_type;
	constexpr auto min = std::numeric_limits<int_type>::min();

	Assert::IsTrue(min == static_cast<int_type>(integer_object{min}));
}
void integer_object_test::test_max_when_construction() {
	using int_type     = integer_object::int_type;
	constexpr auto max = std::numeric_limits<int_type>::max();

	Assert::IsTrue(max == static_cast<int_type>(integer_object{max}));
}
void integer_object_test::test_overflow_max_when_construction() {
	using int_type                = integer_object::int_type;
	using uint_type               = std::make_unsigned_t<int_type>;
	constexpr int_type  max       = std::numeric_limits<int_type>::max();
	constexpr uint_type max_plus1 = static_cast<uint_type>(max) + 1;

	AssertThrows(integer_object_overflows, integer_object{max_plus1});
}

using smaller_type = std::int_least8_t;
static_assert(sizeof(smaller_type) < sizeof(integer_object::int_type),
              "These tests must be run with a type smaller than "
              "integer_object::int_type.");
void integer_object_test::test_min_when_conversion() {
	constexpr auto min = std::numeric_limits<smaller_type>::min();

	Assert::IsTrue(min == static_cast<smaller_type>(integer_object{min}));
}
void integer_object_test::test_max_when_conversion() {
	constexpr auto max = std::numeric_limits<smaller_type>::max();

	Assert::IsTrue(max == static_cast<smaller_type>(integer_object{max}));
}
void integer_object_test::test_overflow_min_when_conversion() {
	constexpr auto min        = std::numeric_limits<smaller_type>::min();
	constexpr auto min_minus1 = static_cast<integer_object::int_type>(min) - 1;

	AssertThrows(integer_object_overflows,
	             static_cast<smaller_type>(integer_object{min_minus1}));
}
void integer_object_test::test_overflow_max_when_conversion() {
	constexpr auto max       = std::numeric_limits<smaller_type>::max();
	constexpr auto max_plus1 = static_cast<integer_object::int_type>(max) + 1;

	AssertThrows(integer_object_overflows,
	             static_cast<smaller_type>(integer_object{max_plus1}));
}
