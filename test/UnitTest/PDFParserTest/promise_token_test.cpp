#include "testtool.h"

namespace tokenizer_test {
[TestClass] public ref class promise_token_test {
public:
	[TestMethod] void test_when_nothrow();
	[TestMethod] void test_when_throw();
};
} // namespace tokenizer_test

#include "AssertThrows.hpp"
#include "pdfparser.tokenizer.hpp"
#include "pdfparser.tokenizer_errors.hpp"

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
