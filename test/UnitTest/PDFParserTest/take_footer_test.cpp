#include "PDFParser.h"
#include "literal_trim.hpp"
#include "take_footer_test.hpp"

#include <sstream>

using namespace pdfparser;
using namespace error_types;
using namespace object_types;
using namespace pdfparser_test;

void take_footer_test::test_sample() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << R"(
xref
0 1
0000000000 65535  f
)"_trimmed;
	stream << R"(
trailer
	<< /Size 1 >>
startxref
0
%%EOF
)";

	stream_parser str_parser(std::move(stream));
	object_pool   obj_pool(str_parser);

	Assert::IsTrue(object_types::dictionary_object{{"Size", 1}} ==
	               str_parser.take_footer(obj_pool));
}
void take_footer_test::test_EOF_EOF_EOL() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << R"(
xref
0 1
0000000000 65535  f
)"_trimmed;
	stream << R"(
trailer
	<< /Size 1 >>
startxref
0
%%EOF)";
	stream << "\r\n";

	stream_parser str_parser(std::move(stream));
	object_pool   obj_pool(str_parser);

	Assert::IsTrue(object_types::dictionary_object{{"Size", 1}} ==
	               str_parser.take_footer(obj_pool));
}
void take_footer_test::test_EOF_EOF_only() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << R"(
xref
0 1
0000000000 65535  f
)"_trimmed;
	stream << R"(
trailer
	<< /Size 1 >>
startxref
0
%%EOF)";

	stream_parser str_parser(std::move(stream));
	object_pool   obj_pool(str_parser);

	Assert::IsTrue(object_types::dictionary_object{{"Size", 1}} ==
	               str_parser.take_footer(obj_pool));
}
void take_footer_test::test_EOF_EOF_not_EOL() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << R"(
xref
0 1
0000000000 65535  f
)"_trimmed;
	stream << R"(
trailer
	<< /Size 1 >>
startxref
0
%%EOF )";

	stream_parser str_parser(std::move(stream));
	object_pool   obj_pool(str_parser);

	try {
		str_parser.take_footer(obj_pool);
	} catch (const error_types::parse_error& parse_e) {
		Assert::IsTrue(parse_error::EOL_not_found == parse_e.code());

		// success
		return;
	}
	Assert::Fail();
}
void take_footer_test::test_EOF_not_EOF() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << R"(
xref
0 1
0000000000 65535  f
)"_trimmed;
	stream << R"(
trailer
	<< /Size 1 >>
startxref
0
%EOF
)";

	stream_parser str_parser(std::move(stream));
	object_pool   obj_pool(str_parser);

	try {
		str_parser.take_footer(obj_pool);
	} catch (const error_types::parse_error& parse_e) {
		Assert::IsTrue(parse_error::keyword_EOF_not_found == parse_e.code());

		// success
		return;
	}
	Assert::Fail();
}
void take_footer_test::test_EOF_SP_EOF() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << R"(
xref
0 1
0000000000 65535  f
)"_trimmed;
	stream << R"(
trailer
	<< /Size 1 >>
startxref
0
 %%EOF
)";

	stream_parser str_parser(std::move(stream));
	object_pool   obj_pool(str_parser);

	try {
		str_parser.take_footer(obj_pool);
	} catch (const error_types::parse_error& parse_e) {
		Assert::IsTrue(parse_error::keyword_EOF_not_found == parse_e.code());

		// success
		return;
	}
	Assert::Fail();
}

void take_footer_test::test_xref_byte_offset_not_found() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "startxref\r\n"
	       << "%%EOF\n";

	stream_parser str_parser(std::move(stream));
	object_pool   obj_pool(str_parser);

	try {
		str_parser.take_footer(obj_pool);
	} catch (const error_types::parse_error& parse_e) {
		Assert::IsTrue(parse_error::xref_byte_offset_not_found == parse_e.code());

		// success
		return;
	}
	Assert::Fail();
}
void take_footer_test::test_startxref_SP_startxref_comment_EOL() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << R"(
xref
0 1
0000000000 65535  f
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

	stream_parser str_parser(std::move(stream));
	object_pool   obj_pool(str_parser);

	Assert::IsTrue(object_types::dictionary_object{{"Size", 1}} ==
	               str_parser.take_footer(obj_pool));
}
void take_footer_test::test_startxref_startxref_not_EOL() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << R"(
xref
0 1
0000000000 65535  f
)"_trimmed;
	stream << R"(
trailer
	<< /Size 1 >>
startxref 0
%%EOF
)";

	stream_parser str_parser(std::move(stream));
	object_pool   obj_pool(str_parser);

	try {
		str_parser.take_footer(obj_pool);
	} catch (const parse_error& parse_e) {
		Assert::IsTrue(parse_error::EOL_not_found == parse_e.code());

		// success
		return;
	}
	Assert::Fail();
}
void take_footer_test::test_startxref_not_startxref() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << R"(
xref
0 1
0000000000 65535  f
)"_trimmed;
	stream << R"(
trailer
	<< /Size 1 >>
sstartxref
0
%%EOF
)";

	stream_parser str_parser(std::move(stream));
	object_pool   obj_pool(str_parser);

	try {
		str_parser.take_footer(obj_pool);
	} catch (const parse_error& parse_e) {
		Assert::IsTrue(parse_error::keyword_startxref_not_found == parse_e.code());

		// success
		return;
	}
	Assert::Fail();
}

void take_footer_test::test_xref_SP_xref_comment_EOL() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << '\0' << "\x09\x0C\x20xref" << '\0'
	       << "\x09\x0C\x20 % this is "
	          "comment.\r\n";
	stream << R"(
0 1
0000000000 65535  f
)"_trimmed;
	stream << R"(
trailer
	<< /Size 1 >>
startxref
0
%%EOF
)";

	stream_parser str_parser(std::move(stream));
	object_pool   obj_pool(str_parser);

	Assert::IsTrue(object_types::dictionary_object{{"Size", 1}} ==
	               str_parser.take_footer(obj_pool));
}
void take_footer_test::test_xref_xref_not_EOL() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << R"(
xref 0 1
0000000000 65535  f
)"_trimmed;
	stream << R"(
trailer
	<< /Size 1 >>
startxref
0
%%EOF
)";

	stream_parser str_parser(std::move(stream));
	object_pool   obj_pool(str_parser);

	try {
		str_parser.take_footer(obj_pool);
	} catch (const parse_error& parse_e) {
		Assert::IsTrue(parse_error::EOL_not_found == parse_e.code());

		// success
		return;
	}
	Assert::Fail();
}
void take_footer_test::test_xref_not_xref() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << R"(
xxref
0 1
0000000000 65535  f
)"_trimmed;
	stream << R"(
trailer
	<< /Size 1 >>
startxref
0
%%EOF
)";

	stream_parser str_parser(std::move(stream));
	object_pool   obj_pool(str_parser);

	try {
		str_parser.take_footer(obj_pool);
	} catch (const parse_error& parse_e) {
		Assert::IsTrue(parse_error::keyword_xref_not_found == parse_e.code());

		// success
		return;
	}
	Assert::Fail();
}
