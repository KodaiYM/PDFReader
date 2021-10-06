#include "literal_trim.hpp"
#include "pdfparser.ipdfstream.hpp"
#include "pdfparser.object_cache.hpp"
#include "take_stream_object_test.hpp"

#include <sstream>

using namespace pdfparser;
using namespace object_types;
using namespace ipdfstream_test::take_object_test;

void take_stream_object_test::test_sample_CRLF() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "<</Length 18>>\n"
	       << "stream\r\n"
	       << " stream contents \n\n"
	       << "endstream";

	ipdfstream str_parser(stream.rdbuf());
	auto       object = str_parser.take_stream_object();
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

	ipdfstream str_parser(stream.rdbuf());
	auto       object = str_parser.take_stream_object();
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
	ipdfstream str_parser(stream.rdbuf());
	str_parser.add_xref_table(
	    xref_types::xref_table{xref_types::xref_inuse_entry{1, 0, 0}});

	auto object = str_parser.take_stream_object();
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

	ipdfstream str_parser(stream.rdbuf());
	try {
		str_parser.take_stream_object();
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

	ipdfstream str_parser(stream.rdbuf());
	try {
		str_parser.take_stream_object();
	} catch (const parse_error& parse_e) {
		Assert::IsTrue(parse_error::stream_data_is_shorter_than_Length ==
		               parse_e.code());

		// success
		return;
	}
	Assert::Fail();
}
