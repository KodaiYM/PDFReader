#pragma once

#include <cstdint>
#include <type_traits>

namespace pdfparser {
enum class whitespace_flags : uint8_t {
	null                      = 1 << 0,
	horizontal_tab            = 1 << 1,
	line_feed                 = 1 << 2,
	form_feed                 = 1 << 3,
	carriage_return           = 1 << 4,
	space                     = 1 << 5,
	comment                   = 1 << 6,
	EOL                       = line_feed | carriage_return,
	any_whitespace_characters = null | horizontal_tab | line_feed | form_feed |
	                            carriage_return | space | EOL,
	any_whitespace_characters_except_EOL = any_whitespace_characters & ~EOL
};
}

/// <summary>
/// Definition of bitwise operations between whitespace_flags
/// </summary>
#pragma region whitespace_flags_bitwise_operations
namespace pdfparser {
constexpr whitespace_flags operator&(whitespace_flags lhs,
                                     whitespace_flags rhs) noexcept {
	using int_type = std::underlying_type_t<whitespace_flags>;
	return static_cast<whitespace_flags>(static_cast<int_type>(lhs) &
	                                     static_cast<int_type>(rhs));
}
constexpr whitespace_flags operator|(whitespace_flags lhs,
                                     whitespace_flags rhs) noexcept {
	using int_type = std::underlying_type_t<whitespace_flags>;
	return static_cast<whitespace_flags>(static_cast<int_type>(lhs) |
	                                     static_cast<int_type>(rhs));
}
constexpr whitespace_flags operator^(whitespace_flags lhs,
                                     whitespace_flags rhs) noexcept {
	using int_type = std::underlying_type_t<whitespace_flags>;
	return static_cast<whitespace_flags>(static_cast<int_type>(lhs) ^
	                                     static_cast<int_type>(rhs));
}
constexpr whitespace_flags& operator&=(whitespace_flags& lhs,
                                       whitespace_flags  rhs) noexcept {
	return lhs = lhs & rhs;
}
constexpr whitespace_flags& operator|=(whitespace_flags& lhs,
                                       whitespace_flags  rhs) noexcept {
	return lhs = lhs | rhs;
}
constexpr whitespace_flags& operator^=(whitespace_flags& lhs,
                                       whitespace_flags  rhs) noexcept {
	return lhs = lhs ^ rhs;
}
constexpr whitespace_flags operator~(whitespace_flags operand) noexcept {
	using int_type = std::underlying_type_t<whitespace_flags>;
	return static_cast<whitespace_flags>(~static_cast<int_type>(operand));
}
} // namespace pdfparser
#pragma endregion // region whitespace_flags_bitwise_operations
