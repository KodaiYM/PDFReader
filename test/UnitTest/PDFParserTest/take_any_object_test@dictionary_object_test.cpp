#include "PDFParser.h"
#include "literal_trim.hpp"
#include "take_any_object_test@dictionary_object_test.hpp"

#include <sstream>

using namespace pdfparser;
using namespace error_types;
using namespace object_types;
using namespace pdfparser_test;

using namespace take_any_object_test;

void dictionary_object_test::test_sample() {
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
	auto          object = str_parser.take_any_object(obj_pool);
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
	               } == std::get<dictionary_object>(object));
}
void dictionary_object_test::test_empty() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "<<>>";

	stream_parser str_parser(std::move(stream));
	object_pool   obj_pool(str_parser);
	auto          object = str_parser.take_any_object(obj_pool);
	Assert::IsTrue(dictionary_object{} == std::get<dictionary_object>(object));
}
void dictionary_object_test::test_null_value() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "<</key null /key2 null /key3 (string) /key4 null>>";

	stream_parser str_parser(std::move(stream));
	object_pool   obj_pool(str_parser);
	auto          object = str_parser.take_any_object(obj_pool);
	Assert::IsTrue(dictionary_object{{"key3", string_object{"string"}}} ==
	               std::get<dictionary_object>(object));
}
void dictionary_object_test::test_key_is_not_name_object() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "<<(string key) (value)>>";

	stream_parser str_parser(std::move(stream));
	object_pool   obj_pool(str_parser);
	try {
		str_parser.take_any_object(obj_pool);
	} catch (const parse_error& parse_e) {
		Assert::IsTrue(parse_error::dictionary_key_is_not_name_object ==
		               parse_e.code());

		// success
		return;
	}
	Assert::Fail();
}
void dictionary_object_test::test_lack_of_double_greater_than_sign() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "<</key (value)> >";

	stream_parser str_parser(std::move(stream));
	object_pool   obj_pool(str_parser);
	try {
		str_parser.take_any_object(obj_pool);
	} catch (const parse_error& parse_e) {
		Assert::IsTrue(parse_error::dictionary_lack_of_double_greater_than_sign ==
		               parse_e.code());

		// success
		return;
	}
	Assert::Fail();
}
