#include "PDFParser.h"
#include "take_any_object_test@eof_test.hpp"

#include <sstream>

using namespace pdfparser;
using namespace error_types;
using namespace object_types;
using namespace pdfparser_test;

using namespace take_any_object_test;

void eof_test::test_eof() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream_parser str_parser(std::move(stream));
	object_pool   obj_pool(str_parser);
	try {
		str_parser.take_any_object(obj_pool);
	} catch (const parse_error& parse_e) {
		Assert::IsTrue(parse_error::object_not_found == parse_e.code());

		// success
		return;
	}
	Assert::Fail();
}
