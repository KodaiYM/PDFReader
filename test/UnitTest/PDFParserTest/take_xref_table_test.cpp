#include "PDFParser.h"
#include "take_xref_table_test.hpp"

#include <sstream>

using namespace pdfparser;
using namespace error_types;
using namespace object_types;
using namespace pdfparser_test;

// test largest xref table
void take_xref_table_test::test_maximum_xref_table() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "xref\n";
	stream << "1 " << std::numeric_limits<xref_types::object_t>::max() << "\n";

	stream_parser str_parser(std::move(stream));
	try {
		str_parser.take_xref_table();
	} catch (const parse_error& parse_e) {
		Assert::IsTrue(parse_error::xref_entry_first_10_digits_invalid ==
		               parse_e.code());

		// success
		return;
	}
	Assert::Fail();
}
// test too large xref table (which occurs std::overflow_error)
void take_xref_table_test::test_overflow() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "xref\n";
	constexpr auto max = std::numeric_limits<xref_types::object_t>::max();
	stream << "1 " << (max / 10) << (max % 10 + 1) << "\n";

	stream_parser str_parser(std::move(stream));
	try {
		str_parser.take_xref_table();
	} catch (std::overflow_error&) {
		// success
		return;
	}
	Assert::Fail();
}