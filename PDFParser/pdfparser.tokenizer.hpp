#pragma once

#include "character_types.hpp"
#include "pdfparser.istream_extended.hpp"
#include "pdfparser.pdftoken.hpp"

#include <algorithm>
#include <cassert>
#include <initializer_list>
#include <optional>
#include <string_view>
#include <utility>

namespace pdfparser {
class tokenizer: public istream_extended {
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

private:
	using base = istream_extended;

public:
	/// <summary>constructor</summary>
	using base::base;

	virtual ~tokenizer() = default;
};
} // namespace pdfparser
