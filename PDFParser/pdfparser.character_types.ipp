#pragma once

#include "character_types.hpp"

#include <string_view>

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
