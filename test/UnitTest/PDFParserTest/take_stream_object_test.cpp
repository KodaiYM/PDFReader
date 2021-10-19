#include "literal_trim.hpp"
#include "pdfparser.object_cache.hpp"
#include "pdfparser.object_stream.hpp"
#include "pdfparser.object_stream_errors.hpp"
#include "take_stream_object_test.hpp"

#include <sstream>

using namespace pdfparser;
using namespace object_types;
using namespace object_stream_test;

void take_stream_object_test::test_sample_CRLF() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "<</Length 18>>\n"
	       << "stream\r\n"
	       << " stream contents \n\n"
	       << "endstream";

	object_stream obj_stream(stream.rdbuf());
	auto          object = obj_stream.take_stream_object();
	Assert::IsTrue(stream_object{dictionary_object{{"Length", 18}},
	                             " stream contents \n"} == object);
	Assert::IsTrue(0 == object.position());
}
void take_stream_object_test::test_sample_LF() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "<</Length 18>>\n"
	       << "stream\n"
	       << " stream contents \n\n"
	       << "endstream";

	object_stream obj_stream(stream.rdbuf());
	auto          object = obj_stream.take_stream_object();
	Assert::IsTrue(stream_object{dictionary_object{{"Length", 18}},
	                             " stream contents \n"} == object);
	Assert::IsTrue(0 == object.position());
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
	object_stream obj_stream(stream.rdbuf());
	obj_stream.add_xref_table(
	    xref_types::xref_table{xref_types::xref_inuse_entry{1, 0, 0}});

	auto object = obj_stream.take_stream_object();
	Assert::IsTrue(
	    stream_object{dictionary_object{{"Length", indirect_reference{1, 0}}},
	                  " stream contents "} == object);
	Assert::IsTrue(beginning_of_stream == object.position());
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

	object_stream obj_stream(stream.rdbuf());
	try {
		obj_stream.take_stream_object();
	} catch (const stream_dictionary_absence_of_Length_entry& e) {
		Assert::IsTrue(0 == e.tell_position());

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

	object_stream obj_stream(stream.rdbuf());
	try {
		obj_stream.take_stream_object();
	} catch (const stream_data_is_shorter_than_Length& e) {
		Assert::IsTrue(22 == e.tell_position());

		// success
		return;
	}
	Assert::Fail();
}
