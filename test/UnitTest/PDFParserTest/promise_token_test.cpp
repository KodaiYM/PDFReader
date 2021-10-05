#include "pdfparser.tokenizer.hpp"
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
	try {
		tknizer.promise_token({"token2", "token3", "token4"});
	} catch (const tokenize_error& tknize_e) {
		Assert::IsTrue(tokenize_error::promise_token_failed == tknize_e.code());

		// success
		return;
	}
	Assert::Fail();
}
