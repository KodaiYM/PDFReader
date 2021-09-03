#pragma once

#include "character_types.hpp"
#include "pdfparser.istream_extended.hpp"
#include "pdfparser.pdftoken.hpp"
#include "pdfparser.tokenize_error.hpp"

#include <algorithm>
#include <cassert>
#include <fstream> // for IntelliSense
#include <initializer_list>
#include <optional>
#include <string_view>
#include <utility>

namespace pdfparser {
template <class InputStreamT>
class tokenizer: public istream_extended<InputStreamT> {
public:
	/// <summary>
	/// check if there is no token
	/// </summary>
	/// <returns>true if and only if there is no token</returns>
	[[nodiscard]] bool no_token() noexcept;

	/// <summary>
	/// consume the token at the current position of the stream
	/// if it matches attempt_token_sv; otherwise, do nothing.
	/// </summary>
	/// <param name="attempt_token_sv">a token string trying to consume</param>
	/// <returns>true if and only if consumed</returns>
	bool attempt_token(std::string_view attempt_token_sv) noexcept;

	/// <summary>
	/// Promise any token string in the promise_token_list to be able to
	/// attempt_token. If none of the tokens can be attempted as token,
	/// it throws tokenize_error(promise_token_failed).
	/// </summary>
	/// <param name="promise_token_list">
	/// list of token strings which one of them is promised to be able to
	/// attempt_token
	/// </param>
	/// <exception cref="pdfparser::tokenize_error(promise_token_failed)">
	/// thrown when none of the token strings can be attempted as token
	/// </exception>
	void
	    promise_token(std::initializer_list<std::string_view> promise_token_list);

	/// <summary>get next token</summary>
	/// <returns>
	/// the next token if it is available; otherwise std::nullopt
	/// </returns>
	std::optional<pdftoken> take_token() noexcept;

public:
	static_assert(std::is_base_of_v<std::istream, InputStreamT>,
	              "template parameter InputStreamT must be a derived class of "
	              "std::istream");

	/// <summary>
	/// move the argument stream and construct
	/// </summary>
	/// <param name="stream">input stream inheriting from std::istream</param>
	/// <exception>
	/// constructor of istream_extended(InputStreamT&amp;&amp;) may throw an
	/// exception.
	/// </exception>
	tokenizer(InputStreamT&& stream);

	/// <summary>
	/// prohibit to move from const rvalue
	/// </summary>
	tokenizer(const InputStreamT&&) = delete;

	/// <summary>
	/// prohibit to copy from lvalue
	/// </summary>
	tokenizer(InputStreamT&) = delete;

	virtual ~tokenizer() = default;
};
} // namespace pdfparser

/// <summary>
/// Definition of member functions of class tokenizer
/// </summary>
namespace pdfparser {
#pragma region special_functions_of_tokenizer
template <class InputStreamT>
tokenizer<InputStreamT>::tokenizer(InputStreamT&& stream)
    : istream_extended(std::move(stream)) {}
#pragma endregion // region special_functions_of_tokenizer

#pragma region member_functions_of_tokenizer
template <class InputStreamT>
[[nodiscard]] bool tokenizer<InputStreamT>::no_token() noexcept {
	auto before_no_token_pos = tell();
	if (take_token().has_value()) {
		seek(before_no_token_pos);
		return false;
	} else {
		seek(before_no_token_pos);
		return true;
	}
}

template <class InputStreamT>
bool tokenizer<InputStreamT>::attempt_token(
    std::string_view attempt_token_sv) noexcept {
	const auto before_take_token_pos = tell();
	if (const auto token = take_token();
	    token.has_value() && attempt_token_sv == token.value()) {
		return true;
	} else {
		seek(before_take_token_pos);
		return false;
	}
}
template <class InputStreamT>
void tokenizer<InputStreamT>::promise_token(
    std::initializer_list<std::string_view> promise_token_list) {
	if (std::any_of(
	        promise_token_list.begin(), promise_token_list.end(),
	        std::bind(&tokenizer::attempt_token, this, std::placeholders::_1))) {
		// found promise_token
		// do nothing
		return;
	} else {
		// not found any of them
		throw tokenize_error(tokenize_error::promise_token_failed);
	}
}
template <class InputStreamT>
std::optional<pdftoken>
    pdfparser::tokenizer<InputStreamT>::take_token() noexcept {
	// HACK: Instead of re-parsing take_token when it is called at the same
	// position, it may be possible to optimize it somewhat by memoizing it. I
	// don't know if it's really faster. Need to verify.

	ignore_if_present(whitespace_flags::any_whitespace_characters |
	                  whitespace_flags::comment);

	if (eof()) {
		return std::nullopt;
	}

	// delimiter token
	if (attempt("<<")) {
		return pdftoken(pdftoken::delimiter_token, "<<");
	}
	if (attempt(">>")) {
		return pdftoken(pdftoken::delimiter_token, ">>");
	}
	if (auto next_ch = peek().value(); is_delimiter(next_ch)) {
		++*this;
		return pdftoken(pdftoken::delimiter_token, std::string{next_ch});
	}

	// regular token
	std::string      regular_characters;
	decltype(peek()) next_ch;
	while ((next_ch = peek()).has_value() && is_regular(next_ch.value())) {
		++*this;
		regular_characters.push_back(next_ch.value());
	}
	assert(!regular_characters.empty());

	return pdftoken(pdftoken::regular_token, std::move(regular_characters));
}
#pragma endregion // region member_functions_of_tokenizer
} // namespace pdfparser
