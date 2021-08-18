#include "PDFParser.h"
#include "take_xref_entry_test.hpp"

#include <iomanip>
#include <sstream>

using namespace pdfparser;
using namespace error_types;
using namespace object_types;
using namespace pdfparser_test;

void take_xref_entry_test::test_SPCR() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	using namespace xref_types;

	constexpr object_t     object_number = 1;
	constexpr auto         next_free     = std::numeric_limits<object_t>::max();
	constexpr generation_t generation_number =
	    std::numeric_limits<generation_t>::max();
	stream << std::setfill('0');
	stream << std::setw(10) << next_free << " " << std::setw(5)
	       << generation_number;
	stream << " f";
	stream << " \r";
	assert(stream.str().length() == 20);

	stream_parser str_parser(std::move(stream));
	Assert::IsTrue(
	    str_parser.take_xref_entry(object_number) ==
	    xref_entry{xref_free_entry{object_number, generation_number, next_free}});
}
void take_xref_entry_test::test_SPLF() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	using namespace xref_types;

	constexpr object_t     object_number = 0;
	constexpr auto         next_free     = 0;
	constexpr generation_t generation_number =
	    std::numeric_limits<generation_t>::max();
	stream << std::setfill('0');
	stream << std::setw(10) << next_free << " " << std::setw(5)
	       << generation_number;
	stream << " f";
	stream << " \n";
	assert(stream.str().length() == 20);

	stream_parser str_parser(std::move(stream));
	Assert::IsTrue(
	    str_parser.take_xref_entry(object_number) ==
	    xref_entry{xref_free_entry{object_number, generation_number, next_free}});
}
void take_xref_entry_test::test_CRLF() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	using namespace xref_types;

	constexpr object_t     object_number = std::numeric_limits<object_t>::max();
	constexpr auto         byte_offset   = 9999999999;
	constexpr generation_t generation_number =
	    std::numeric_limits<generation_t>::max();
	stream << std::setfill('0');
	stream << std::setw(10) << byte_offset << " " << std::setw(5)
	       << generation_number;
	stream << " n";
	stream << "\r\n";
	assert(stream.str().length() == 20);

	stream_parser str_parser(std::move(stream));
	Assert::IsTrue(str_parser.take_xref_entry(object_number) ==
	               xref_entry{xref_inuse_entry{object_number, generation_number,
	                                           byte_offset}});
}
