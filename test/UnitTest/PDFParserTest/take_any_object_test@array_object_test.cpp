#include "PDFParser.h"
#include "literal_trim.hpp"
#include "take_any_object_test@array_object_test.hpp"

#include <sstream>

using namespace pdfparser;
using namespace error_types;
using namespace object_types;
using namespace pdfparser_test;

using namespace take_any_object_test;

void array_object_test::test_valid_array() {
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

	stream_parser str_parser(std::move(stream));
	object_pool   obj_pool(str_parser);
	auto          object = str_parser.take_any_object(obj_pool);

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
	        null                                                  //
	    } == std::get<array_object>(object));
}
void array_object_test::test_empty_array() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "[]";

	stream_parser str_parser(std::move(stream));
	object_pool   obj_pool(str_parser);
	auto          object = str_parser.take_any_object(obj_pool);
	Assert::IsTrue(array_object{} == std::get<array_object>(object));
}
void array_object_test::test_lack_of_right_square_bracket() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "[0 1 2";

	stream_parser str_parser(std::move(stream));
	object_pool   obj_pool(str_parser);
	try {
		str_parser.take_any_object(obj_pool);
	} catch (const parse_error& parse_e) {
		Assert::IsTrue(parse_error::array_lack_of_right_square_bracket ==
		               parse_e.code());

		// success
		return;
	}
	Assert::Fail();
}
