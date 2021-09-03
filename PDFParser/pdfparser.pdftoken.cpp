#include "pdfparser.pdftoken.hpp"

#include <type_traits>

/// <summary>Definition of member functions of class pdftoken</summary>
namespace pdfparser {
#pragma region special_functions_of_pdftoken
pdftoken::pdftoken(token_type type, std::string token_str) noexcept
    : m_type(std::move(type)), m_token_str(std::move(token_str)) {
	static_assert(std::is_nothrow_move_constructible_v<std::string>);
}
#pragma endregion // region special_functions_of_pdftoken

#pragma region member_functions_of_pdftoken
auto pdftoken::type() const noexcept -> token_type {
	return m_type;
}

std::string pdftoken::str() const {
	return m_token_str;
}
pdftoken::operator std::string() const {
	return str();
}

std::string_view pdftoken::sview() const noexcept {
	static_assert(
	    std::is_nothrow_constructible_v<std::string_view, decltype(m_token_str)>);
	return m_token_str;
}
pdftoken::operator std::string_view() const noexcept {
	return sview();
}
#pragma endregion // region member_functions_of_pdftoken
} // namespace pdfparser
