#include "PDFParser.h"
#include "literal_trim.hpp"
#include "take_indirect_object_test.hpp"

#include <sstream>

using namespace pdfparser;
using namespace error_types;
using namespace object_types;
using namespace pdfparser_test;

using namespace xref_types;

void take_indirect_object_test::test_sample() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << R"(
7 0 obj
	<< /Length 8 0 R >>
stream
)"_trimmed;
	std::string stream_contents = R"(
	BT
		/F1 12 Tf
		72 712 Td
		(A stream with an indirect length) Tj
	ET
)"_trimmed;

	stream << '\n' << stream_contents;
	stream << R"(
endstream
endobj
)";
	std::streamoff offset_of_objnum8 = stream.tellp();
	stream << R"(
8 0 obj
)";
	stream << '\t' << stream_contents.length();
	stream << R"(
endobj
)";

	stream_parser str_parser(std::move(stream));
	object_pool   obj_pool(str_parser);
	obj_pool.add_xref_table(
	    xref_table{xref_inuse_entry{8, 0, offset_of_objnum8}});
	auto object = str_parser.take_indirect_object(obj_pool, 7, 0);
	Assert::IsTrue(
	    stream_object{dictionary_object{
	                      {"Length", static_cast<int>(stream_contents.length())}},
	                  stream_contents} == std::get<stream_object>(object));
}
void take_indirect_object_test::test_inconsistent_object_number() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << R"(
7 0 obj
	(Brillig)
endobj
)"_trimmed;

	stream_parser str_parser(std::move(stream));
	object_pool   obj_pool(str_parser);
	try {
		str_parser.take_indirect_object(obj_pool, 8, 0);
	} catch (const parse_error& parse_e) {
		Assert::IsTrue(
		    parse_error::indirect_object_is_inconsistent_with_xref_table ==
		    parse_e.code());

		// success
		return;
	}
	Assert::Fail();
}
void take_indirect_object_test::test_inconsistent_generation_number() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << R"(
7 0 obj
	(Brillig)
endobj
)"_trimmed;

	stream_parser str_parser(std::move(stream));
	object_pool   obj_pool(str_parser);
	try {
		str_parser.take_indirect_object(obj_pool, 7, 1);
	} catch (const parse_error& parse_e) {
		Assert::IsTrue(
		    parse_error::indirect_object_is_inconsistent_with_xref_table ==
		    parse_e.code());

		// success
		return;
	}
	Assert::Fail();
}
void take_indirect_object_test::test_keyword_obj_not_found() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << R"(
7 0 ob 
	(Brillig)
endobj
)"_trimmed;

	stream_parser str_parser(std::move(stream));
	object_pool   obj_pool(str_parser);
	try {
		str_parser.take_indirect_object(obj_pool, 7, 0);
	} catch (const parse_error& parse_e) {
		Assert::IsTrue(parse_error::keyword_obj_not_found == parse_e.code());

		// success
		return;
	}
	Assert::Fail();
}
void take_indirect_object_test::test_refers_indirect_reference() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << R"(
7 0 obj
	1 0 R
endobj
)"_trimmed;

	stream_parser str_parser(std::move(stream));
	object_pool   obj_pool(str_parser);
	try {
		str_parser.take_indirect_object(obj_pool, 7, 0);
	} catch (const parse_error& parse_e) {
		Assert::IsTrue(parse_error::indirect_object_refers_indirect_reference ==
		               parse_e.code());

		// success
		return;
	}
	Assert::Fail();
}
void take_indirect_object_test::test_keyword_endobj_not_found() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << R"(
7 0 obj
	(Brillig)
endob
)"_trimmed;

	stream_parser str_parser(std::move(stream));
	object_pool   obj_pool(str_parser);
	try {
		str_parser.take_indirect_object(obj_pool, 7, 0);
	} catch (const parse_error& parse_e) {
		Assert::IsTrue(parse_error::keyword_endobj_not_found == parse_e.code());

		// success
		return;
	}
	Assert::Fail();
}
