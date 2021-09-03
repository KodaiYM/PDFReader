#pragma once

#include <string_view>

namespace pdfparser {
/// <summary>
/// check if character is a whitespace character
/// </summary>
/// <param name="character">character to check</param>
/// <returns>
/// true if and only if character is one of \0, \t, \n, \f, \r, or space(\x20)
/// </returns>
[[nodiscard]] constexpr bool is_whitespace(char character) noexcept;

/// <summary>
/// check if character is a delimiter character
/// </summary>
/// <param name="character">character to check</param>
/// <returns>
/// true if and only if character is one of ( ) &lt; &gt; [ ] { } / %
/// </returns>
[[nodiscard]] constexpr bool is_delimiter(char character) noexcept;

/// <summary>
/// check if character is a regular character
/// </summary>
/// <param name="character">character to check</param>
/// <returns>
/// true if and only if character is
/// neither a whitespace character nor a delimiter character
/// </returns>
[[nodiscard]] constexpr bool is_regular(char character) noexcept;
} // namespace pdfparser

/// <summary>
/// functions to determine character types
/// </summary>
#pragma region character_type_functions
namespace pdfparser {
[[nodiscard]] constexpr bool is_whitespace(char character) noexcept {
	using namespace std::string_view_literals;
	return "\0\t\n\f\r "sv.find(character) != std::string_view::npos;
}
[[nodiscard]] constexpr bool is_delimiter(char character) noexcept {
	using namespace std::string_view_literals;
	return "()<>[]{}/%"sv.find(character) != std::string_view::npos;
}
[[nodiscard]] constexpr bool is_regular(char character) noexcept {
	return !is_whitespace(character) && !is_delimiter(character);
}
} // namespace pdfparser
#pragma endregion // region character_type_functions
