#include "PDFParser.h"
#include "require_test.hpp"

#include <sstream>

using namespace pdfparser;
using namespace error_types;
using namespace object_types;
using namespace pdfparser_test;

void require_test::test_EOF_EOF_EOL() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "%%EOF\r\n";
	std::streamoff expected = stream.tellp();

	stream_parser str_parser(std::move(stream));
	str_parser.require(require_type::keyword_EOF);
	Assert::IsTrue(expected == str_parser.tell());
}
void require_test::test_EOF_EOF_only() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "%%EOF";
	std::streamoff expected = stream.tellp();

	stream_parser str_parser(std::move(stream));
	str_parser.require(require_type::keyword_EOF);
	Assert::IsTrue(expected == str_parser.tell());
}
void require_test::test_EOF_EOF_not_EOL() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "%%EOF ";

	stream_parser str_parser(std::move(stream));
	try {
		str_parser.require(require_type::keyword_EOF);
	} catch (const error_types::parse_error& e) {
		Assert::IsTrue(parse_error::EOL_not_found == e.code());

		// success
		return;
	}
	Assert::Fail();
}
void require_test::test_EOF_not_EOF() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "%EOF\n";

	stream_parser str_parser(std::move(stream));
	try {
		str_parser.require(require_type::keyword_EOF);
	} catch (const error_types::parse_error& e) {
		Assert::IsTrue(parse_error::keyword_EOF_not_found == e.code());

		// success
		return;
	}
	Assert::Fail();
}
void require_test::test_EOF_SP_EOF() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << " %%EOF\n";

	stream_parser str_parser(std::move(stream));
	try {
		str_parser.require(require_type::keyword_EOF);
	} catch (const error_types::parse_error& e) {
		Assert::IsTrue(parse_error::keyword_EOF_not_found == e.code());

		// success
		return;
	}
	Assert::Fail();
}
void require_test::test_EOF_end_of_line() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream_parser str_parser(std::move(stream));
	try {
		str_parser.require(require_type::keyword_EOF);
	} catch (const error_types::parse_error& e) {
		Assert::IsTrue(parse_error::keyword_EOF_not_found == e.code());

		// success
		return;
	}
	Assert::Fail();
}

void require_test::test_startxref_startxref_EOL() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "startxref\r\n";
	std::streamoff expected = stream.tellp();

	stream_parser str_parser(std::move(stream));
	str_parser.require(require_type::keyword_startxref);
	Assert::IsTrue(expected == str_parser.tell());
}
void require_test::test_startxref_SP_startxref_comment_EOL() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << '\0' << "\x09\x0C\x20startxref" << '\0'
	       << "\x09\x0C\x20 % this is "
	          "comment.\r\n";
	std::streamoff expected = stream.tellp();

	stream_parser str_parser(std::move(stream));
	str_parser.require(require_type::keyword_startxref);
	Assert::IsTrue(expected == str_parser.tell());
}
void require_test::test_startxref_startxref_only() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "startxref";

	stream_parser str_parser(std::move(stream));
	try {
		str_parser.require(require_type::keyword_startxref);
	} catch (const parse_error& parse_e) {
		Assert::IsTrue(parse_error::EOL_not_found == parse_e.code());

		// success
		return;
	}
	Assert::Fail();
}
void require_test::test_startxref_startxref_not_EOL() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "startxref invalidtokens\n";

	stream_parser str_parser(std::move(stream));
	try {
		str_parser.require(require_type::keyword_startxref);
	} catch (const parse_error& parse_e) {
		Assert::IsTrue(parse_error::EOL_not_found == parse_e.code());

		// success
		return;
	}
	Assert::Fail();
}
void require_test::test_startxref_not_startxref() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "sstartxref\n";

	stream_parser str_parser(std::move(stream));
	try {
		str_parser.require(require_type::keyword_startxref);
	} catch (const parse_error& parse_e) {
		Assert::IsTrue(parse_error::keyword_startxref_not_found == parse_e.code());

		// success
		return;
	}
	Assert::Fail();
}
void require_test::test_startxref_end_of_line() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream_parser str_parser(std::move(stream));
	try {
		str_parser.require(require_type::keyword_startxref);
	} catch (const parse_error& parse_e) {
		Assert::IsTrue(parse_error::keyword_startxref_not_found == parse_e.code());

		// success
		return;
	}
	Assert::Fail();
}

void require_test::test_xref_xref_EOL() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "xref\r\n";
	std::streamoff expected = stream.tellp();

	stream_parser str_parser(std::move(stream));
	str_parser.require(require_type::keyword_xref);
	Assert::IsTrue(expected == str_parser.tell());
}
void require_test::test_xref_SP_xref_comment_EOL() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << '\0' << "\x09\x0C\x20xref" << '\0'
	       << "\x09\x0C\x20 % this is "
	          "comment.\r\n";
	std::streamoff expected = stream.tellp();

	stream_parser str_parser(std::move(stream));
	str_parser.require(require_type::keyword_xref);
	Assert::IsTrue(expected == str_parser.tell());
}
void require_test::test_xref_xref_only() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "xref";

	stream_parser str_parser(std::move(stream));
	try {
		str_parser.require(require_type::keyword_xref);
	} catch (const parse_error& parse_e) {
		Assert::IsTrue(parse_error::EOL_not_found == parse_e.code());

		// success
		return;
	}
	Assert::Fail();
}
void require_test::test_xref_xref_not_EOL() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "xref invalidtokens\n";

	stream_parser str_parser(std::move(stream));
	try {
		str_parser.require(require_type::keyword_xref);
	} catch (const parse_error& parse_e) {
		Assert::IsTrue(parse_error::EOL_not_found == parse_e.code());

		// success
		return;
	}
	Assert::Fail();
}
void require_test::test_xref_not_xref() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "xxref\n";

	stream_parser str_parser(std::move(stream));
	try {
		str_parser.require(require_type::keyword_xref);
	} catch (const parse_error& parse_e) {
		Assert::IsTrue(parse_error::keyword_xref_not_found == parse_e.code());

		// success
		return;
	}
	Assert::Fail();
}
void require_test::test_xref_end_of_line() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream_parser str_parser(std::move(stream));
	try {
		str_parser.require(require_type::keyword_xref);
	} catch (const parse_error& parse_e) {
		Assert::IsTrue(parse_error::keyword_xref_not_found == parse_e.code());

		// success
		return;
	}
	Assert::Fail();
}

void require_test::test_space_space_only() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << ' ';
	std::streamoff expected = stream.tellp();

	stream_parser str_parser(std::move(stream));
	str_parser.require(require_type::space);
	Assert::IsTrue(expected == str_parser.tell());
}
void require_test::test_space_not_space() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "\n";

	stream_parser str_parser(std::move(stream));
	try {
		str_parser.require(require_type::space);
	} catch (const parse_error& parse_e) {
		Assert::IsTrue(parse_error::space_not_found == parse_e.code());

		// success
		return;
	}
	Assert::Fail();
}
void require_test::test_space_end_of_line() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream_parser str_parser(std::move(stream));
	try {
		str_parser.require(require_type::space);
	} catch (const parse_error& parse_e) {
		Assert::IsTrue(parse_error::space_not_found == parse_e.code());

		// success
		return;
	}
	Assert::Fail();
}
