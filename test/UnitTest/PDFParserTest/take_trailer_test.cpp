#include "literal_trim.hpp"
#include "pdfparser.document_parser.hpp"
#include "pdfparser.object_pool.hpp"
#include "take_trailer_test.hpp"

#include <sstream>

using namespace pdfparser;
using namespace object_types;
using namespace document_parser_test;

void take_trailer_test::test_sample_trailer() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);
	stream << R"(
trailer
	<< /Size 22
	   /Root 2 0 R
	   /Info 1 0 R
	   /ID [ <81b14aafa313db63dbd6f981e49f94f4>
	         <81b14aafa313db63dbd6f981e49f94f4>
	       ]
	>>
)"_trimmed;

	document_parser str_parser(std::move(stream));
	object_pool     obj_pool(str_parser);

	// check if no-throw
	str_parser.take_trailer(obj_pool);
}
void take_trailer_test::test_no_white_space_after_keyword_trailer() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);
	stream << "trailer<<>>";

	document_parser str_parser(std::move(stream));
	object_pool     obj_pool(str_parser);

	// check if no-throw
	str_parser.take_trailer(obj_pool);
}
