#include "testtool.h"

namespace ipdfstream_test {
[TestClass] public ref class take_xref_entry_test {
public:
	[TestMethod] void test_keyword_invalid();
};
} // namespace ipdfstream_test

#include "AssertThrows.hpp"
#include "pdfparser.ipdfstream.hpp"
#include "pdfparser.ipdfstream_errors.hpp"

using namespace ipdfstream_test;
using namespace pdfparser;

void take_xref_entry_test::test_keyword_invalid() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "100 0 fn";

	ipdfstream str_parser(stream.rdbuf());

	AssertThrows(xref_entry_keyword_invalid, str_parser.take_xref_entry(1));
}
