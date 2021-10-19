#pragma once

#include "pdfparser.portion_of_stream.hpp"

namespace pdfparser {

#pragma region portion_of_stream
inline portion_of_stream::portion_of_stream(std::streampos position) noexcept
    : m_position(std::move(position)) {}
inline portion_of_stream::~portion_of_stream() noexcept {}

inline const std::streampos& portion_of_stream::position() const& noexcept {
	return m_position;
}
inline std::streampos portion_of_stream::position() && noexcept {
	return std::move(m_position);
}
#pragma endregion // region portion_of_stream

} // namespace pdfparser
