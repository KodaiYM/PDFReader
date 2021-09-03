#include "literal_trim.hpp"
#include "pdfparser.object_pool.hpp"
#include "pdfparser.stream_parser.hpp"
#include "take_dictionary_object_test.hpp"

#include <sstream>

using namespace pdfparser;
using namespace object_types;
using namespace pdfparser_test;

void take_dictionary_object_test::test_sample() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << R"(
<< /Type /Example
   /SubType /DictionaryExample
   /Version 0.01
   /IntegerItem 12
   /StringItem (a string)
   /Subdictionary << /Item1 0.4
                     /Item2 true
                     /LastItem (not!)
                     /VeryLastItem (OK)
                  >>
>>
)"_trimmed;

	stream_parser str_parser(std::move(stream));
	object_pool   obj_pool(str_parser);
	auto          object = str_parser.take_dictionary_object(obj_pool);
	Assert::IsTrue(dictionary_object{
	                   {"Type", name_object{"Example"}},              // these
	                   {"SubType", name_object{"DictionaryExample"}}, // comments
	                   {"Version", 0.01},                             // are
	                   {"IntegerItem", 12},                           // for
	                   {"StringItem", string_object{"a string"}},     // format
	                   {"Subdictionary",                              // align
	                    dictionary_object{
	                        {"Item1", 0.4},                       //
	                        {"Item2", true},                      //
	                        {"LastItem", string_object{"not!"}},  //
	                        {"VeryLastItem", string_object{"OK"}} //
	                    }}                                        //
	               } == object);
}
void take_dictionary_object_test::test_empty() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "<<>>";

	stream_parser str_parser(std::move(stream));
	object_pool   obj_pool(str_parser);
	auto          object = str_parser.take_dictionary_object(obj_pool);
	Assert::IsTrue(dictionary_object{} == dictionary_object(object));
}
void take_dictionary_object_test::test_null_value() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "<</key null /key2 null /key3 (string) /key4 null>>";

	stream_parser str_parser(std::move(stream));
	object_pool   obj_pool(str_parser);
	auto          object = str_parser.take_dictionary_object(obj_pool);
	Assert::IsTrue(dictionary_object{{"key3", string_object{"string"}}} ==
	               object);
}
void take_dictionary_object_test::test_lack_of_double_greater_than_sign() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "<</key (value)";

	stream_parser str_parser(std::move(stream));
	object_pool   obj_pool(str_parser);
	try {
		str_parser.take_dictionary_object(obj_pool);
	} catch (const parse_error& parse_e) {
		Assert::IsTrue(parse_error::dictionary_lack_of_double_greater_than_sign ==
		               parse_e.code());

		// success
		return;
	}
	Assert::Fail();
}