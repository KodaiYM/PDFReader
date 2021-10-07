#include "literal_trim.hpp"
#include "pdfparser.ipdfstream.hpp"
#include "pdfparser.object_cache.hpp"
#include "take_trailer_test.hpp"

#include <sstream>

using namespace pdfparser;
using namespace object_types;
using namespace ipdfstream_test;

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

	ipdfstream str_parser(stream.rdbuf());

	// check if no-throw
	str_parser.take_trailer();
}
void take_trailer_test::test_no_white_space_after_keyword_trailer() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);
	stream << "trailer<<>>";

	ipdfstream str_parser(stream.rdbuf());

	// check if no-throw
	str_parser.take_trailer();
}
