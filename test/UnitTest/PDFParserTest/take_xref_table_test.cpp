#include "testtool.h"

namespace ipdfstream_test {
[TestClass] public ref class take_xref_table_test {
public:
	[TestMethod] void test_maximum_xref_table();
	[TestMethod] void test_overflow();
};
} // namespace ipdfstream_test

#include "AssertThrows.hpp"
#include "pdfparser.ipdfstream.hpp"
#include "pdfparser.ipdfstream_errors.hpp"

#include <sstream>

using namespace pdfparser;
using namespace object_types;
using namespace ipdfstream_test;

// test largest xref table
void take_xref_table_test::test_maximum_xref_table() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "xref\n";
	stream << "1 " << std::numeric_limits<xref_types::object_t>::max() << "\n";

	ipdfstream str_parser(stream.rdbuf());
	try {
		str_parser.take_xref_table();
	} catch (object_number_overflow_in_xref_table&) {
		Assert::Fail(); // overflow error -> fail
	} catch (...) {
		// success
	}
}
// test too large xref table (which occurs std::overflow_error)
void take_xref_table_test::test_overflow() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "xref\n";
	constexpr auto max = std::numeric_limits<xref_types::object_t>::max();
	stream << "1 " << (max / 10) << (max % 10 + 1) << "\n";

	ipdfstream str_parser(stream.rdbuf());

	AssertThrows(onstream_integer_object_overflows, str_parser.take_xref_table());
}
