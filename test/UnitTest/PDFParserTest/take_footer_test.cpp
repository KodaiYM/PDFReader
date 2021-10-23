#include "testtool.h"

namespace ipdfstream_test {
[TestClass] public ref class take_footer_test {
public:
	[TestMethod] void test_sample();
	[TestMethod] void test_EOF_EOF_EOL();
	[TestMethod] void test_EOF_EOF_only();

	[TestMethod] void test_startxref_SP_startxref_comment_EOL();
	[TestMethod] void test_xref_SP_xref_comment_EOL();
};
} // namespace ipdfstream_test

#include "literal_trim.hpp"
#include "pdfparser.ipdfstream.hpp"
#include "pdfparser.ipdfstream_errors.hpp"
#include "pdfparser.object_cache.hpp"

#include <sstream>

using namespace pdfparser;
using namespace object_types;
using namespace ipdfstream_test;

void take_footer_test::test_sample() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << R"(
xref
0 1
0000000000 65535 f 
)"_trimmed;
	stream << R"(
trailer
	<< /Size 1 >>
startxref
0
%%EOF
)";

	ipdfstream str_parser(stream.rdbuf());

	Assert::IsTrue(object_types::dictionary_object{{"Size", 1}} ==
	               str_parser.take_footer());
}
void take_footer_test::test_EOF_EOF_EOL() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << R"(
xref
0 1
0000000000 65535 f 
)"_trimmed;
	stream << R"(
trailer
	<< /Size 1 >>
startxref
0
%%EOF)";
	stream << "\r\n";

	ipdfstream str_parser(stream.rdbuf());

	Assert::IsTrue(object_types::dictionary_object{{"Size", 1}} ==
	               str_parser.take_footer());
}
void take_footer_test::test_EOF_EOF_only() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << R"(
xref
0 1
0000000000 65535 f 
)"_trimmed;
	stream << R"(
trailer
	<< /Size 1 >>
startxref
0
%%EOF)";

	ipdfstream str_parser(stream.rdbuf());

	Assert::IsTrue(object_types::dictionary_object{{"Size", 1}} ==
	               str_parser.take_footer());
}

void take_footer_test::test_startxref_SP_startxref_comment_EOL() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << R"(
xref
0 1
0000000000 65535 f 
)"_trimmed;
	stream << R"(
trailer
	<< /Size 1 >>
)";
	stream << '\0' << "\x09\x0C\x20startxref" << '\0'
	       << "\x09\x0C\x20 % this is "
	          "comment.\r\n";
	stream << R"(
0
%%EOF
)"_trimmed
	       << '\n';

	ipdfstream str_parser(stream.rdbuf());

	Assert::IsTrue(object_types::dictionary_object{{"Size", 1}} ==
	               str_parser.take_footer());
}
void take_footer_test::test_xref_SP_xref_comment_EOL() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << '\0' << "\x09\x0C\x20xref" << '\0'
	       << "\x09\x0C\x20 % this is "
	          "comment.\r\n";
	stream << R"(
0 1
0000000000 65535 f 
)"_trimmed;
	stream << R"(
trailer
	<< /Size 1 >>
startxref
0
%%EOF
)";

	ipdfstream str_parser(stream.rdbuf());

	Assert::IsTrue(object_types::dictionary_object{{"Size", 1}} ==
	               str_parser.take_footer());
}
