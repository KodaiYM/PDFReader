#include "pdfparser.pdftoken.hpp"

#include <type_traits>

namespace pdfparser {
pdftoken::pdftoken(std::streampos position, token_type type,
                   std::string token_str) noexcept
    : portion_of_stream(std::move(position)), m_type(std::move(type)),
      m_token_str(std::move(token_str)) {}

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
} // namespace pdfparser
