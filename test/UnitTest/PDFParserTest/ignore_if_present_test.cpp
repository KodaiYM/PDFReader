#include "PDFParser.h"
#include "ignore_if_present_test.hpp"

#include <sstream>

using namespace pdfparser;
using namespace error_types;
using namespace object_types;
using namespace pdfparser_test;

void ignore_if_present_test::test_null_only() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << '\0' << " \t\n\f\r ";

	stream_parser str_parser(std::move(stream));
	str_parser.ignore_if_present(ignore_flag::null);
	Assert::IsTrue(1 == str_parser.tell());
}
void ignore_if_present_test::test_line_feed_only() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "\n\r";

	stream_parser str_parser(std::move(stream));
	str_parser.ignore_if_present(ignore_flag::line_feed);
	Assert::IsTrue(1 == str_parser.tell());
}
void ignore_if_present_test::test_form_feed_only() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << '\f' << " \t\n\f\r ";

	stream_parser str_parser(std::move(stream));
	str_parser.ignore_if_present(ignore_flag::form_feed);
	Assert::IsTrue(1 == str_parser.tell());
}
void ignore_if_present_test::test_carriage_return_only() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "\r\n";

	stream_parser str_parser(std::move(stream));
	str_parser.ignore_if_present(ignore_flag::carriage_return);
	Assert::IsTrue(1 == str_parser.tell());
}
void ignore_if_present_test::test_space_only() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << ' ' << "\t\n\f\r ";

	stream_parser str_parser(std::move(stream));
	str_parser.ignore_if_present(ignore_flag::space);
	Assert::IsTrue(1 == str_parser.tell());
}
void ignore_if_present_test::test_comment_only() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "% this is comment section";
	std::streamoff expected = stream.tellp();

	stream << "\n\r ";

	stream_parser str_parser(std::move(stream));
	str_parser.ignore_if_present(ignore_flag::comment);
	Assert::IsTrue(expected == str_parser.tell());
}
void ignore_if_present_test::test_EOL_only() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "\r\n ";

	stream_parser str_parser(std::move(stream));
	str_parser.ignore_if_present(ignore_flag::EOL);
	Assert::IsTrue(2 == str_parser.tell());
}
void ignore_if_present_test::test_any_whitespace_characters() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << '\0' << "\t\n\f\r ";
	std::streamoff expected = stream.tellp();

	stream_parser str_parser(std::move(stream));
	str_parser.ignore_if_present(ignore_flag::any_whitespace_characters);
	Assert::IsTrue(expected == str_parser.tell());
}
void ignore_if_present_test::test_all_whitespaces_including_comment() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << '\0' << "\t\n\f\r %comment section\r\n"
	       << "\t\n\r\n " << '\0' << "\r\n %comment 2 section!\r"
	       << "% last comment !\n ";
	std::streamoff expected = stream.tellp();

	stream_parser str_parser(std::move(stream));
	str_parser.ignore_if_present(ignore_flag::any_whitespace_characters |
	                             ignore_flag::comment);
	Assert::IsTrue(expected == str_parser.tell());
}
void ignore_if_present_test::test_nothing_to_ignore() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "don't ignore\r\n";

	stream_parser str_parser(std::move(stream));
	str_parser.ignore_if_present(ignore_flag::any_whitespace_characters |
	                             ignore_flag::comment);
	Assert::IsTrue(0 == str_parser.tell());
}
void ignore_if_present_test::test_no_flags() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << '\0' << "\t\n\f\r ";

	stream_parser str_parser(std::move(stream));
	str_parser.ignore_if_present(ignore_flag::null & ~ignore_flag::null);
	Assert::IsTrue(0 == str_parser.tell());
}
