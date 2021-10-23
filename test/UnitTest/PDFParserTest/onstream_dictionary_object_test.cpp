#include "testtool.h"

namespace object_types_test {
[TestClass] public ref class onstream_dictionary_object_test {
public:
	[TestMethod] void test_at_out_of_range();
};
} // namespace object_types_test

#include "AssertThrows.hpp"
#include "pdfparser.object_types.hpp"

using namespace pdfparser;
using namespace pdfparser::object_types;

using namespace object_types_test;

void onstream_dictionary_object_test::test_at_out_of_range() {
	AssertThrows(onstream_dictionary_out_of_range,
	             onstream_dictionary_object{0, {}}.at("key"));
}
