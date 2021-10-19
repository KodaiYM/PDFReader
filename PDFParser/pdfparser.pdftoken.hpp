#pragma once

#include "pdfparser.portion_of_stream.hpp"

#include <string>
#include <string_view>

namespace pdfparser {
struct pdftoken: portion_of_stream {
	enum token_type { delimiter_token, regular_token };

public:
	/// <returns>this token type</returns>
	[[nodiscard]] token_type type() const noexcept;

	/// <returns>this token string</returns>
	[[nodiscard]] std::string str() const;
	                          operator std::string() const;

	/// <returns>view of this token string</returns>
	[[nodiscard]] std::string_view sview() const noexcept;
	                               operator std::string_view() const noexcept;

public:
	pdftoken(std::streampos position, token_type type,
	         std::string token_str) noexcept;

private:
	/// <summary>token type of this token</summary>
	token_type m_type;

	/// <summary>token string</summary>
	std::string m_token_str;
};
} // namespace pdfparser
