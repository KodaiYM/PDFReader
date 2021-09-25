#include "literal_trim.hpp"
#include "pdfparser.object_pool.hpp"
#include "pdfparser.stream_parser.hpp"
#include "take_stream_object_test.hpp"

#include <sstream>

using namespace pdfparser;
using namespace object_types;
using namespace stream_parser_test::take_object_test;

void take_stream_object_test::test_sample_CRLF() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "<</Length 18>>\n"
	       << "stream\r\n"
	       << " stream contents \n\n"
	       << "endstream";

	document_parser str_parser(std::move(stream));
	object_pool     obj_pool(str_parser);
	auto            object = str_parser.take_stream_object(obj_pool);
	Assert::IsTrue(stream_object{dictionary_object{{"Length", 18}},
	                             " stream contents \n"} == object);
}
void take_stream_object_test::test_sample_LF() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "<</Length 18>>\n"
	       << "stream\n"
	       << " stream contents \n\n"
	       << "endstream";

	document_parser str_parser(std::move(stream));
	object_pool     obj_pool(str_parser);
	auto            object = str_parser.take_stream_object(obj_pool);
	Assert::IsTrue(stream_object{dictionary_object{{"Length", 18}},
	                             " stream contents \n"} == object);
}
void take_stream_object_test::test_indirect_reference_Length() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << R"(
1 0 obj
 17
endobj
)"_trimmed;
	stream << R"(
2 0 obj
)";
	auto beginning_of_stream = stream.tellp();
	stream << R"(
<</Length 1 0 R>>
stream
 stream contents 
endstream
endobj
)"_trimmed;

	stream.seekg(beginning_of_stream);
	document_parser str_parser(std::move(stream));
	object_pool     obj_pool(str_parser);
	obj_pool.add_xref_table(
	    xref_types::xref_table{xref_types::xref_inuse_entry{1, 0, 0}});

	auto object = str_parser.take_stream_object(obj_pool);
	Assert::IsTrue(
	    stream_object{dictionary_object{{"Length", indirect_reference{1, 0}}},
	                  " stream contents "} == object);
}
void take_stream_object_test::test_absence_of_Length_entry() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << R"(
<<>>
stream
 stream contents 
endstream
)"_trimmed;

	document_parser str_parser(std::move(stream));
	object_pool     obj_pool(str_parser);
	try {
		str_parser.take_stream_object(obj_pool);
	} catch (const parse_error& parse_e) {
		Assert::IsTrue(parse_error::stream_dictionary_absence_of_Length_entry ==
		               parse_e.code());

		// success
		return;
	}
	Assert::Fail();
}
void take_stream_object_test::test_data_is_shorter_than_Length() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << R"(
<</Length 30>>
stream
 stream contents 
endstream
)"_trimmed;

	document_parser str_parser(std::move(stream));
	object_pool     obj_pool(str_parser);
	try {
		str_parser.take_stream_object(obj_pool);
	} catch (const parse_error& parse_e) {
		Assert::IsTrue(parse_error::stream_data_is_shorter_than_Length ==
		               parse_e.code());

		// success
		return;
	}
	Assert::Fail();
}
