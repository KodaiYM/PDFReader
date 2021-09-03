#include "pdfparser.istream_extended.hpp"
#include "pdfparser.istream_extended_error.hpp"
#include "seek_test.hpp"

#include <sstream>

using namespace pdfparser;
using namespace istream_extended_test;

void seek_test::test_to_between_CRLF() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "\r\n"
	       << "\n"
	       << "\r\r"
	       << "\r\n";

	istream_extended str_extended(std::move(stream));
	str_extended.seek(6);
	Assert::IsTrue(6 == str_extended.tell());
}
void seek_test::test_to_EOF() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "test";

	istream_extended str_extended(std::move(stream));
	str_extended.seek(4);
	Assert::IsTrue(4 == str_extended.tell());
}
void seek_test::test_over_EOF() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "test";

	istream_extended str_extended(std::move(stream));
	try {
		str_extended.seek(5);
	} catch (const istream_extended_error& parse_e) {
		Assert::IsTrue(istream_extended_error::failed_to_seek == parse_e.code());

		// success
		return;
	}
	Assert::Fail();
}
