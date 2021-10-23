#include "AssertThrows.hpp"
#include "pdfparser.tokenizer.hpp"
#include "pdfparser.tokenizer_errors.hpp"
#include "promise_token_test.hpp"

#include <sstream>

using namespace pdfparser;
using namespace tokenizer_test;

void promise_token_test::test_when_nothrow() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "token";

	tokenizer tknizer(stream.rdbuf());
	// check if no-throw
	tknizer.promise_token({"dummy1", "token", "dummy3"});
}
void promise_token_test::test_when_throw() {
	std::stringstream stream(std::ios_base::in | std::ios_base::out |
	                         std::ios_base::binary);

	stream << "token1 token2 token3";

	tokenizer tknizer(stream.rdbuf());

	AssertThrows(promise_token_failed,
	             tknizer.promise_token({"token2", "token3", "token4"}));
}
