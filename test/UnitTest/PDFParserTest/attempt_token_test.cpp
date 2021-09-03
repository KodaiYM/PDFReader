#include "attempt_token_test.hpp"
#include "pdfparser.tokenizer.hpp"

#include <sstream>

using namespace pdfparser;
using namespace tokenizer_test;

void attempt_token_test::test_when_true() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "token";

	tokenizer tknizer(std::move(stream));

	Assert::IsTrue(tknizer.attempt_token("token"));
}
void attempt_token_test::test_when_false() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "token";

	tokenizer tknizer(std::move(stream));

	Assert::IsTrue(tknizer.tell() == 0);
	Assert::IsFalse(tknizer.attempt_token("dummy"));
	Assert::IsTrue(tknizer.tell() == 0);
}
