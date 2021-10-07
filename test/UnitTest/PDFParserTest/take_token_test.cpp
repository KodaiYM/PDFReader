#include "literal_trim.hpp"
#include "pdfparser.tokenizer.hpp"
#include "take_token_test.hpp"

#include <sstream>
#include <string_view>

using namespace pdfparser;
using namespace tokenizer_test;
using namespace std::string_view_literals;

void take_token_test::test_sp_separated_regular_token() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "token1 token2";

	tokenizer tknizer(stream.rdbuf());
	Assert::IsTrue("token1"sv == tknizer.take_token());
}
void take_token_test::test_eof_separated_regular_token() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "token";

	tokenizer tknizer(stream.rdbuf());
	Assert::IsTrue("token"sv == tknizer.take_token());
}
void take_token_test::test_delimiter_separated_regular_token() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "token1/token2";

	tokenizer tknizer(stream.rdbuf());
	Assert::IsTrue("token1"sv == tknizer.take_token());
}

void take_token_test::test_double_delimiter_token() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "<<something";

	tokenizer tknizer(stream.rdbuf());
	Assert::IsTrue("<<"sv == tknizer.take_token());
}
void take_token_test::test_single_delimiter_token() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "/something";

	tokenizer tknizer(stream.rdbuf());
	Assert::IsTrue("/"sv == tknizer.take_token());
}

void take_token_test::test_comment_ignorable() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << R"(
% comment1
% comment2
token%comment3
)"_trimmed;

	tokenizer tknizer(stream.rdbuf());
	Assert::IsTrue("token"sv == tknizer.take_token());
}

void take_token_test::test_whitespace_eof() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << " ";

	tokenizer tknizer(stream.rdbuf());
	Assert::IsFalse(tknizer.take_token().has_value());
}
void take_token_test::test_at_eof() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	tokenizer tknizer(stream.rdbuf());
	Assert::IsFalse(tknizer.take_token().has_value());
}
