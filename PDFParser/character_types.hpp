#pragma once

namespace pdfparser {
/// <summary>
/// check if character is a whitespace character
/// </summary>
/// <returns>
/// true if and only if character is one of \0, \t, \n, \f, \r, or space(\x20)
/// </returns>
[[nodiscard]] constexpr bool is_whitespace(char character) noexcept;

/// <summary>
/// check if character is a delimiter character
/// </summary>
/// <returns>
/// true if and only if character is one of ( ) &lt; &gt; [ ] { } / %
/// </returns>
[[nodiscard]] constexpr bool is_delimiter(char character) noexcept;

/// <summary>
/// check if character is a regular character
/// </summary>
/// <returns>
/// true if and only if character is
/// neither a whitespace character nor a delimiter character
/// </returns>
[[nodiscard]] constexpr bool is_regular(char character) noexcept;
} // namespace pdfparser

#include "pdfparser.character_types.ipp"
