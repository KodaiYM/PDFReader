#include "testtool.h"

namespace object_types_test {
[TestClass] public ref class array_object_test {
public:
	[TestMethod] void test_at_out_of_range_when_empty();
	[TestMethod] void test_at_out_of_range();
};
} // namespace object_types_test

#include "AssertThrows.hpp"
#include "pdfparser.object_types.hpp"

#include <cstdint>
#include <limits>

using namespace pdfparser;
using namespace pdfparser::object_types;

using namespace object_types_test;

void array_object_test::test_at_out_of_range_when_empty() {
	AssertThrows(array_out_of_range, array_object{}.at(0));
}

void array_object_test::test_at_out_of_range() {
	AssertThrows(array_out_of_range, array_object{1, 2}.at(2));
}
