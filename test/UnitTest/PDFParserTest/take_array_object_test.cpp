#include "AssertThrows.hpp"
#include "literal_trim.hpp"
#include "pdfparser.object_cache.hpp"
#include "pdfparser.object_stream.hpp"
#include "pdfparser.object_stream_errors.hpp"
#include "take_array_object_test.hpp"

#include <sstream>

using namespace pdfparser;
using namespace object_types;
using namespace object_stream_test;

void take_array_object_test::test_simple_array() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "[1 2 3]";

	object_stream obj_stream(stream.rdbuf());
	auto          object = obj_stream.take_array_object();
	Assert::IsTrue(array_object{1, 2, 3} == object);
	Assert::IsTrue(0 == object.position());
}
void take_array_object_test::test_valid_array() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << R"(
[ << /test (test) 
     /test2 (test2) >>
  [1 [2 3]]
  (str]ing])
	1.1
  1
  .1
	1.
  0 1 R
  true
  false
  null
]
)"_trimmed;

	object_stream obj_stream(stream.rdbuf());
	auto          object = obj_stream.take_array_object();

	Assert::IsTrue(
	    array_object{
	        dictionary_object{{"test", string_object{"test"}},
	                          {"test2", string_object{"test2"}}}, // these
	        array_object{1, array_object{2, 3}},                  // comments
	        string_object{"str]ing]"},                            // are
	        real_object{1.1},                                     // for
	        integer_object{1},                                    // format
	        real_object{0.1},                                     // align
	        real_object{1.0},                                     //
	        indirect_reference{0, 1},                             //
	        boolean_object{true},                                 //
	        boolean_object{false},                                //
	        null_object{}                                         //
	    } == object);
	Assert::IsTrue(0 == object.position());
}
void take_array_object_test::test_empty_array() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "[]";

	object_stream obj_stream(stream.rdbuf());
	auto          object = obj_stream.take_array_object();
	Assert::IsTrue(array_object{} == object && 0 == object.position());
}
void take_array_object_test::test_lack_of_right_square_bracket() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "[0 1 2";

	object_stream obj_stream(stream.rdbuf());

	AssertThrows(array_lack_of_right_square_bracket,
	             obj_stream.take_array_object());
}
