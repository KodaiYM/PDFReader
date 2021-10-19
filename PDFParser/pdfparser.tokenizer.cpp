#include "pdfparser.tokenizer.hpp"
#include "pdfparser.tokenizer_errors.hpp"

using namespace pdfparser;

[[nodiscard]] bool tokenizer::no_token() noexcept {
	auto before_no_token_pos = tell();
	if (take_token().has_value()) {
		seek(before_no_token_pos);
		return false;
	} else {
		seek(before_no_token_pos);
		return true;
	}
}

bool tokenizer::attempt_token(std::string_view attempt_token_sv) noexcept {
	const auto before_take_token_pos = tell();
	if (const auto token = take_token();
	    token.has_value() && attempt_token_sv == token.value()) {
		return true;
	} else {
		seek(before_take_token_pos);
		return false;
	}
}
void tokenizer::promise_token(
    std::initializer_list<std::string_view> promise_token_list) {
	ignore_if_present(whitespace_flags::any_whitespace_characters |
	                  whitespace_flags::comment);

	if (std::any_of(
	        promise_token_list.begin(), promise_token_list.end(),
	        std::bind(&tokenizer::attempt_token, this, std::placeholders::_1))) {
		// found promise_token
		// do nothing
		return;
	} else {
		// not found any of them
		throw promise_token_failed(tell(), promise_token_list.begin(),
		                           promise_token_list.end());
	}
}
std::optional<pdftoken> pdfparser::tokenizer::take_token() noexcept {
	// HACK: Instead of re-parsing take_token when it is called at the same
	// position, it may be possible to optimize it somewhat by memoizing it. I
	// don't know if it's really faster. Need to verify.

	ignore_if_present(whitespace_flags::any_whitespace_characters |
	                  whitespace_flags::comment);

	if (eof()) {
		return std::nullopt;
	}

	// delimiter token
	auto position = tell();
	if (attempt("<<")) {
		return pdftoken(position, pdftoken::delimiter_token, "<<");
	}
	if (attempt(">>")) {
		return pdftoken(position, pdftoken::delimiter_token, ">>");
	}
	if (auto next_ch = peek().value(); is_delimiter(next_ch)) {
		++*this;
		return pdftoken(position, pdftoken::delimiter_token, std::string{next_ch});
	}

	// regular token
	std::string      regular_characters;
	decltype(peek()) next_ch;
	while ((next_ch = peek()).has_value() && is_regular(next_ch.value())) {
		++*this;
		regular_characters.push_back(next_ch.value());
	}
	assert(!regular_characters.empty());

	return pdftoken(position, pdftoken::regular_token,
	                std::move(regular_characters));
}
