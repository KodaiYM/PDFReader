#pragma once

#include "pdfparser.pdftoken.hpp"

#include <type_traits>

namespace pdfparser {
inline pdftoken::pdftoken(std::streampos position, token_type type,
                          std::string token_str) noexcept
    : portion_of_stream(std::move(position)), m_type(std::move(type)),
      m_token_str(std::move(token_str)) {}

inline auto pdftoken::type() const noexcept -> token_type {
	return m_type;
}

inline const std::string& pdftoken::str() const& noexcept {
	return m_token_str;
}
inline std::string pdftoken::str() && noexcept {
	return std::move(m_token_str);
}

inline pdftoken::operator const std::string&() const volatile& noexcept {
	return const_cast<const pdftoken*>(this)->str();
}
inline pdftoken::operator std::string() && noexcept {
	return static_cast<pdftoken&&>(*this).str();
}

inline std::string_view pdftoken::sview() const noexcept {
	static_assert(
	    std::is_nothrow_constructible_v<std::string_view, decltype(m_token_str)>);
	return m_token_str;
}
inline pdftoken::operator std::string_view() const noexcept {
	return sview();
}
} // namespace pdfparser
