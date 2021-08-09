#include "PDFParser.h"
#include "take_trailer_test.hpp"

#include <sstream>

using namespace pdfparser;
using namespace error_types;
using namespace object_types;
using namespace pdfparser_test;

void take_trailer_test::test_sample_trailer() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);
	// clang-format off
	stream << "\
trailer" R"(
	<< /Size 22
	   /Root 2 0 R
	   /Info 1 0 R
	   /ID [ <81b14aafa313db63dbd6f981e49f94f4>
	         <81b14aafa313db63dbd6f981e49f94f4>
	       ]
	>>)"; // clang-format on

	stream_parser str_parser(std::move(stream));
	object_pool   obj_pool(str_parser);
	try {
		str_parser.take_trailer(obj_pool);

		// success
		return;
	} catch (...) {}
	Assert::Fail();
}
void take_trailer_test::test_no_white_space_after_keyword_trailer() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);
	stream << "trailer<<>>";

	stream_parser str_parser(std::move(stream));
	object_pool   obj_pool(str_parser);
	try {
		str_parser.take_trailer(obj_pool);

		// success
		return;
	} catch (...) {}
	Assert::Fail();
}
void take_trailer_test::test_white_space_before_keyword_trailer() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);
	stream << " trailer<<>>";

	stream_parser str_parser(std::move(stream));
	object_pool   obj_pool(str_parser);
	try {
		str_parser.take_trailer(obj_pool);
	} catch (const parse_error& parse_e) {
		Assert::IsTrue(parse_error::keyword_trailer_not_found == parse_e.code());

		// success
		return;
	}
	Assert::Fail();
}
void take_trailer_test::test_no_dictionary_object() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);
	stream << "trailer <deadbeef>";

	stream_parser str_parser(std::move(stream));
	object_pool   obj_pool(str_parser);
	try {
		str_parser.take_trailer(obj_pool);
	} catch (const parse_error& parse_e) {
		Assert::IsTrue(parse_error::trailer_dictionary_not_found == parse_e.code());

		// success
		return;
	}
	Assert::Fail();
}
void take_trailer_test::test_no_keyword_trailer() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);
	stream << "traile<<>>";

	stream_parser str_parser(std::move(stream));
	object_pool   obj_pool(str_parser);
	try {
		str_parser.take_trailer(obj_pool);
	} catch (const parse_error& parse_e) {
		Assert::IsTrue(parse_error::keyword_trailer_not_found == parse_e.code());

		// success
		return;
	}
	Assert::Fail();
}
