#include "testtool.h"

namespace istream_extended_test {
[TestClass] public ref class ignore_if_present_test {
public:
	[TestMethod] void test_null_only();
	[TestMethod] void test_line_feed_only();
	[TestMethod] void test_form_feed_only();
	[TestMethod] void test_carriage_return_only();
	[TestMethod] void test_space_only();
	[TestMethod] void test_comment_only();
	[TestMethod] void test_EOL_only();
	[TestMethod] void test_any_whitespace_characters();
	[TestMethod] void test_all_whitespaces_including_comment();
	[TestMethod] void test_nothing_to_ignore();
	[TestMethod] void test_no_flags();
};
} // namespace istream_extended_test

#include "pdfparser.istream_extended.hpp"

#include <sstream>

using namespace pdfparser;
using namespace istream_extended_test;

void ignore_if_present_test::test_null_only() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << '\0' << " \t\n\f\r ";

	istream_extended str_extended(stream.rdbuf());
	str_extended.ignore_if_present(whitespace_flags::null);
	Assert::IsTrue(1 == str_extended.tell());
}
void ignore_if_present_test::test_line_feed_only() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "\n\r";

	istream_extended str_extended(stream.rdbuf());
	str_extended.ignore_if_present(whitespace_flags::line_feed);
	Assert::IsTrue(1 == str_extended.tell());
}
void ignore_if_present_test::test_form_feed_only() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << '\f' << " \t\n\f\r ";

	istream_extended str_extended(stream.rdbuf());
	str_extended.ignore_if_present(whitespace_flags::form_feed);
	Assert::IsTrue(1 == str_extended.tell());
}
void ignore_if_present_test::test_carriage_return_only() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "\r\n";

	istream_extended str_extended(stream.rdbuf());
	str_extended.ignore_if_present(whitespace_flags::carriage_return);
	Assert::IsTrue(1 == str_extended.tell());
}
void ignore_if_present_test::test_space_only() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << ' ' << "\t\n\f\r ";

	istream_extended str_extended(stream.rdbuf());
	str_extended.ignore_if_present(whitespace_flags::space);
	Assert::IsTrue(1 == str_extended.tell());
}
void ignore_if_present_test::test_comment_only() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "% this is comment section";
	std::streamoff expected = stream.tellp();

	stream << "\n\r ";

	istream_extended str_extended(stream.rdbuf());
	str_extended.ignore_if_present(whitespace_flags::comment);
	Assert::IsTrue(expected == str_extended.tell());
}
void ignore_if_present_test::test_EOL_only() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "\r\n ";

	istream_extended str_extended(stream.rdbuf());
	str_extended.ignore_if_present(whitespace_flags::EOL);
	Assert::IsTrue(2 == str_extended.tell());
}
void ignore_if_present_test::test_any_whitespace_characters() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << '\0' << "\t\n\f\r ";
	std::streamoff expected = stream.tellp();

	istream_extended str_extended(stream.rdbuf());
	str_extended.ignore_if_present(whitespace_flags::any_whitespace_characters);
	Assert::IsTrue(expected == str_extended.tell());
}
void ignore_if_present_test::test_all_whitespaces_including_comment() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << '\0' << "\t\n\f\r %comment section\r\n"
	       << "\t\n\r\n " << '\0' << "\r\n %comment 2 section!\r"
	       << "% last comment !\n ";
	std::streamoff expected = stream.tellp();

	istream_extended str_extended(stream.rdbuf());
	str_extended.ignore_if_present(whitespace_flags::any_whitespace_characters |
	                               whitespace_flags::comment);
	Assert::IsTrue(expected == str_extended.tell());
}
void ignore_if_present_test::test_nothing_to_ignore() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "don't ignore\r\n";

	istream_extended str_extended(stream.rdbuf());
	str_extended.ignore_if_present(whitespace_flags::any_whitespace_characters |
	                               whitespace_flags::comment);
	Assert::IsTrue(0 == str_extended.tell());
}
void ignore_if_present_test::test_no_flags() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << '\0' << "\t\n\f\r ";

	istream_extended str_extended(stream.rdbuf());
	str_extended.ignore_if_present(whitespace_flags::null &
	                               ~whitespace_flags::null);
	Assert::IsTrue(0 == str_extended.tell());
}
