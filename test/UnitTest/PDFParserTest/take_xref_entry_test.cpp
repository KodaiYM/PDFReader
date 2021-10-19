#include "pdfparser.ipdfstream.hpp"
#include "pdfparser.ipdfstream_errors.hpp"
#include "take_xref_entry_test.hpp"

using namespace ipdfstream_test;
using namespace pdfparser;

void take_xref_entry_test::test_keyword_invalid() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "100 0 fn";

	ipdfstream str_parser(stream.rdbuf());
	try {
		str_parser.take_xref_entry(1);
	} catch (const xref_entry_keyword_invalid& e) {
		Assert::IsTrue(6 == e.tell_position());

		// success
		return;
	}
	Assert::Fail();
}
