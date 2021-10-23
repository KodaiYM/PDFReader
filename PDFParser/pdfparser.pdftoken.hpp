#pragma once

#include "pdfparser.portion_of_stream.hpp"

#include <string>
#include <string_view>

namespace pdfparser {
struct pdftoken: portion_of_stream {
	enum token_type { delimiter_token, regular_token };

public:
	/// <returns>this token type</returns>
	[[nodiscard]] inline token_type type() const noexcept;

	/// <returns>this token string</returns>
	[[nodiscard]] inline const std::string& str() const& noexcept;
	[[nodiscard]] inline std::string        str() && noexcept;

	/// <returns>this token string</returns>
	inline operator const std::string&() const volatile& noexcept;
	/// <returns>this token string</returns>
	inline operator std::string() && noexcept;

	/// <returns>view of this token string</returns>
	[[nodiscard]] inline std::string_view sview() const noexcept;

	/// <returns>view of this token string</returns>
	inline operator std::string_view() const noexcept;

public:
	inline pdftoken(std::streampos position, token_type type,
	                std::string token_str) noexcept;

private:
	/// <summary>token type of this token</summary>
	token_type m_type;

	/// <summary>token string</summary>
	std::string m_token_str;
};
static_assert(!std::is_invocable_v<
              decltype(&pdftoken::operator const std::string&), pdftoken&&>);
} // namespace pdfparser

#include "pdfparser.pdftoken.ipp"
